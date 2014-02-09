Langton's Own LUCID Calibration Analysis for Timepix (LOLCAT)
=============================================================

* Issued under the (2-clause BSD license)
* Written by Hector Stalker

##Introduction

This is a program for:

1. Generating calibration graphs for medipix chips in the LUCID project using calibration data.
1. Generating Wiki formatted table entries containing the metadata of the input data files.

The code is written in `C++11`.

##Compilation instructions

If you are prompted for a password anywhere in this setup process, please enter it.

* The user will need the GCC compiler installed, which can be done by opening the terminal and typing:

        sudo apt-get install build-essential

* The user will need CMake installed, which can be done by opening the terminal and typing:

        sudo apt-get install cmake

* The user will need boost filesystem installed, which can be done on Ubuntu (or Linux in general) by entering the terminal and typing:

        sudo apt-get install libboost-all-dev

* The user will need git installed, which can be done by typing into the terminal:

        sudo apt-get install git


* The repository can be grabbed from the github repository using git by opening the terminal and typing:

        git clone git://github.com/avaritia/lolcat.git

* After cloning the repository to your computer, you need to enter the directory of the lolcat project by typing:

        cd ./lolcat

* Then in that same terminal window type:

        cmake -G "Unix Makefiles"

Wait until the terminal says that 'the build files have been written....' etc.

* Next type:

        make all

and wait for the program to finish running (it will say 'Built target lolcat')


Now lolcat has been successfully compiled, and resides in the /bin/ folder in the lolcat project directory.

##Generating the documentation

To generate the code documentation, type:

    doxygen Doxygen

OR

Use the cmake generated makefile, like so (so after you've run: 'cmake -G "Unix Makefiles"'):

    make doc

To view the code documentation, open a web browser from the command line and navigate to the automatically-generated html page:

    firefox doc/html/index.html &

##Running the code

### Test data

To run on the test data supplied with the repo, use the following commands:

    ./bin/lolcat -t data/TestDetector/Data/Ikrum\=1\,\ TpxClock\=16\ 55\ Fe/ClusterLogAll.txt

This will output a wiki table entry to the terminal that you can copy and paste directly into the wiki.

    ./bin/lolcat -c data/TestDetector/Data/Ikrum\=1\,\ TpxClock\=16\ 55\ Fe/ClusterLogAll.txt

### General running

Run like so from the main lolcat directory using the terminal (see compilation instructions to find out how to get there):

    ./bin/lolcat -t "DetectorName/Data/SettingsUsed/ClusterLogAll.txt"

in order to generate a wiki table entry.

--OR--

    ./bin/lolcat -c "DetectorName/Data/SettingsUsed/ClusterLogAll.txt"

in order to generate calibration information.


##A note on the data folder structures

Currently there is a strictly specified folder layout.

This exact layout is necessary in order to generate a correct table entry, as the settings and detector name are
grabbed from the path to the cluster log given into the program at run-time.

The path should be formatted so that: `DetectorName/Data/SettingsUsed`, where only DetectorName and SettingsUsed change
depending on the dataset.

This is done for ease of use, as it matches the layout of the current test
datasets.


##External libraries used

* Boost - `Filesystem` - Under the Boost Licence - [Filesystem](http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/index.htm)
