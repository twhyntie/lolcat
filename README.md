Langton's Own LUCID Calibration Analysis for Timepix (LOLCAT)
=============================================================

* Issued nder the (2-clause BSD license)
* Written by Hector Stalker

##Introduction

This is a program for:

1. Generating calibration graphs for medipix chips in the LUCID project using calibration data.
1. Generating Wiki formatted table entries containing the metadata of the input data files.

The code is written in `C++11`.

##Compilation instructions

PLEASE COMPLETE THIS.

##Generating the documentation

To generate the code documentation, type:

    doxygen Doxygen

To view the code documentation, open a web browser from the command line and navigate to the automatically-generated html page:

    firefox doc/html/index.html &

##Running the code

Run like so:

    lolcat -t "DetectorName/Data/SettingsUsed/ClusterLogAll.txt"

in order to generate a wiki table entry.

--OR--

    lolcat -c "DetectorName/Data/SettingsUsed/ClusterLogAll.txt"

in order to generate calibration information.

Note the specified folder layout. This exact layout is necessary in order to generate a correct table entry,
as the settings and detector name are grabbed from the path tot he cluster log given into the program.
The path should be formatted so that: DetectorName/Data/SettingsUsed, Where only DetectorName and SettingsUsed
change depending on the dataset. This is done for eas of use, as it matches the layout of the current test
datasets.

##External libraries used

* Boost - `Filesystem` - Under the Boost Licence - (Filesystem)[http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/index.htm]
