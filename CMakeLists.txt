cmake_minimum_required(VERSION 2.8.11)
project(domtester)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

set(domtester_SRC
  src/main.cpp
  src/domtester.cpp
)

# Create code from a list of Qt designer ui files.
#set(CMAKE_AUTOUIC ON) # use this if you have CMake 3.x instead of the following
qt5_wrap_ui(domtester_SRC src/domtester.ui)

# Tell CMake to create the helloworld executable
add_executable(domtester ${domtester_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(domtester Qt5::Widgets)

# Install the executable
install(TARGETS domtester DESTINATION bin)
