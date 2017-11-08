/**
 * domtester
 * Copyright (C) 2017 IdleAir
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "domtester.h"
#include "ui_domtester.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QByteArray>
#include <sstream>

using namespace std;




static inline string &ltrim(string &s)
{
	s.erase(s.begin(), find_if_not(s.begin(), s.end(), [] (int c){return isspace(c); }));
	return s;
}

static inline string &rtrim(string &s)
{
	s.erase(find_if_not(s.rbegin(), s.rend(), [] (int c){return isspace(c); }).base(), s.end());
	return s;
}

static inline string trim(const string &s)
{
	string t = s;

	return ltrim(rtrim(t));
}

void domtester::badBlocksFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	ui->result->setText("FINISHED");
	ui->statusLabel->setText("Insert a disk...");
}

void domtester::newData()
{
	QByteArray buffer = badBlocks->readAllStandardError();

	string newBuff = " ";

	newBuff.append(buffer.data());

	string percent = newBuff.substr(0, 7);
	int where = newBuff.find("(");
	int endw = newBuff.find(" errors)");

	string errors = newBuff.substr(newBuff.find("(") + 1, endw - where);

	percent = trim(percent);
	int p = 0;
	try {
		p = stoi(percent);
	} catch (exception e) {
		if (percent == "done") p = 100;
	}



	ui->progressBar->setValue(p);
	if (percent != "done") ui->errorCount->setText(errors.c_str());
}

void domtester::newDisk()
{
	programArgs.clear();
	string captionText = "Disk " + lastDevice + " inserted..";




	ui->statusLabel->setText(QString(captionText.c_str()));
	ui->result->setText("WAIT...");

	programArgs << "-n" << "-s" << lastDevice.c_str();
	badBlocks->start(program, programArgs);



	ui->result->setText("RUNNING");
}

void domtester::diskRemoved()
{
}


void domtester::updatePmap()
{
	ifstream file;



	file.open("/proc/partitions");

	string data;
	tmap.clear();

	while (getline(file, data)) {
		//cout << data << endl;
		int result = data.rfind(' ');
		if (result == string::npos) continue;

		string partition = data.substr(result + 1, 4);

		if (partition.size() != 3) continue;

		if (pmap.find(partition) == pmap.end()) {
			pmap[partition] = "/dev/" + partition;
			cout << "ADDED  " << partition << endl;
			lastDevice = "/dev/" + partition;
			if (!firstRun) emit deviceAdded();
		}

		tmap[partition] = "/dev/" + partition;
	}

	file.close();




	for (auto pmapIt : pmap) {
		string part = pmapIt.first;
		if (tmap.find(part) == tmap.end()) {
			cout << "REMOVED: " << part << endl;
			pmap.erase(part);


			if (!firstRun) emit deviceRemoved();
		}
	}


	if (firstRun) firstRun = false;
}

domtester::domtester(QWidget *parent)
{
	partitionChecker = new QTimer(this);
	connect(partitionChecker, SIGNAL(timeout()), this, SLOT(updatePmap()));
	partitionChecker->start(1000);

	connect(this, SIGNAL(deviceAdded()), this, SLOT(newDisk()));
	connect(this, SIGNAL(deviceRemoved()), this, SLOT(diskRemoved()));

	programArgs << "-n" << "-s" << lastDevice.c_str();

	badBlocks = new QProcess(this);

	connect(badBlocks, SIGNAL(readyReadStandardError()), this, SLOT(newData()));
	connect(badBlocks, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(badBlocksFinished(int, QProcess::ExitStatus)));


	ui = new Ui::Form();
	ui->setupUi(this);
	show();
}

domtester::~domtester()
{
	delete ui;
}
