#!/bin/bash

projectDir=$(pwd)

sudo apt update
sudo apt upgrade
sudo apt install git cmake

sudo rm /var/lib/dpkg/status
sudo touch /var/lib/dpkg/status

cd $HOME

git clone https://github.com/WiringPi/WiringPi.git
rm -r WiringPi
cd WiringPi
./build

cd $projectDir

rm CMakeLists.txt
touch CMakeLists.txt
rm main.cpp
touch main.cpp
rm -r build
mkdir build

echo "cmake_minimum_required(VERSION 3.16.0)" >> CMakeLists.txt
echo "project(prog)" >> CMakeLists.txt
echo "add_executable(\${PROJECT_NAME} main.cpp piTools.cpp)" >> CMakeLists.txt
echo "find_library(REQUIRED wiringPi)" >> CMakeLists.txt
echo "target_link_libraries(\${PROJECT_NAME} PRIVATE wiringPi pthread)" >> CMakeLists.txt
echo "target_include_directories(\${PROJECT_NAME} PRIVATE "$projectDir")" >> CMakeLists.txt

echo "#include <piTools.h>" >> main.cpp
echo "" >> main.cpp
echo "int main(int args, char* argc[])" >> main.cpp
echo "{" >> main.cpp
echo "" >> main.cpp
echo "" >> main.cpp
echo "	// Main program loop" >> main.cpp
echo "	bool running = true;" >> main.cpp
echo "	while (running)" >> main.cpp
echo "	{" >> main.cpp
echo "" >> main.cpp
echo "	}" >> main.cpp
echo "" >> main.cpp
echo "	return 0;" >> main.cpp
echo "}" >> main.cpp

cd build
cmake ..
make

cd ..
