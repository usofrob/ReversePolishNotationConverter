#!/bin/bash

function showError {
	echo "Error during $1"
	exit 1
}

# I wanted to use plant uml, but this project might be too simple to be worth it.
# mkdir -p bin/ || showError "Creating bin directory"
# wget http://sourceforge.net/projects/plantuml/files/plantuml.jar/download -O bin/plantuml.jar || showError "Downloading plantuml.jar"

sudo apt-get install check || showError "Installing check"

mkdir -p bin
