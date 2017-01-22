#!/bin/bash

function showError {
	echo "Error during $1"
	exit 1
}

# I wanted to use plant uml, but this project might be too simple to be worth it.
# mkdir -p bin/ || showError "Creating bin directory"
# wget http://sourceforge.net/projects/plantuml/files/plantuml.jar/download -O bin/plantuml.jar || showError "Downloading plantuml.jar"

CHECK_INSTALLED=$(apt-cache policy check | grep Installed | grep '(none)')
# This will return nothing (empty string) if the application is already installed
if [ ! -z "${CHECK_INSTALLED}" ]; then
	sudo apt-get install check || showError "Installing check"
else
	echo "check is already installed"
fi

VALGRIND_INSTALLED=$(apt-cache policy valgrind | grep Installed | grep '(none)')
# This will return nothing (empty string) if the application is already installed
if [ ! -z "${VALGRIND_INSTALLED}" ]; then
	sudo apt-get install valgrind || showError "Installing valgrind"
else
	echo "valgrind is already installed"
fi

rm -rf bin lib
mkdir -p bin lib
