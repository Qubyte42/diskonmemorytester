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


#ifndef DOMTESTER_H
#define DOMTESTER_H

#include <QMainWindow>
#include "ui_domtester.h"
#include <sys/inotify.h>
#include <map>
#include <string>
#include <QTimer>
#include <QProcess>
#include <QStringList>


namespace Ui {
class domtester;
}

class domtester : public QMainWindow
{
Q_OBJECT

public:
explicit domtester(QWidget *parent = 0);
~domtester();

private:
Ui::Form *ui;
std::map<std::string, std::string> pmap;
std::map<std::string, std::string> tmap;
std::string lastDevice = "";
bool firstRun = true;
QTimer *partitionChecker = nullptr;
QProcess *badBlocks = nullptr;
QString program = "badblocks";
QStringList programArgs;
signals:
void deviceAdded();
void deviceRemoved();
private slots:
virtual void updatePmap();
virtual void newDisk();
virtual void diskRemoved();
virtual void newData();
virtual void badBlocksFinished(int exitCode, QProcess::ExitStatus exitStatus);
};
#endif // DOMTESTER_H
