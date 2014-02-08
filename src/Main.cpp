/**
 * @file        Main.cpp
 * @author      Hector Stalker <hstalker0@gmail.com>
 * @version     0.1
 *
 * @brief       Main driver code for the data calibration application
 *
 * @copyright   This file is under the BSD 2-Clause license <br>
 * For conditions of distribution and use, see: <br>
 * http://opensource.org/licenses/BSD-2-Clause <br>
 * or read the 'LICENSE' file distributed with this code <br>
 */

// C++ headers
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <cstdlib>
#include <cstring>
// My headers
#include <Pixel.hpp> // For the pixel data type
#include <Frame.hpp> // For the frame data type
#include <TextFileReader.hpp> // For the text file reader class
#include <TableEntryGen.hpp> // For the class for handling Wiki table entry generation

// Constant for the name of the log file
static const char LOG_FILE_NAME[] = "log.txt";


/**
 * @brief Outputs the details of the frame to the given ostream
 * @param log The ostream to log into
 * @param frame A reference to the frame to be logged
 * @param frameNumber The number of the frame
 * @return Nothing
 */
template <class T>
inline void logFrameDetails(std::ostream& log,
                            Frame<T> const& frame,
                            int frameNumber)
{
    // Log all frame details to the given ostream
    log << "Frame no: " << frameNumber << "\n";
    log << frame << "\n";
}


/**
 * @brief       Main function which drives the application <br>
 * Handles:<br>
 * (1) Reading the input <br>
 * (2) Formatting and internalizing the input <br>
 * (3) Emitting the details of the dataset in a Wiki table entry format
 * (4) Constructing the graphs (TODO)
 * @param argc  The number of arguments given to the program when run
 * @param argv  An array of strings which are the arguments given
 * @return      Returns the classic K&R integer (as one would expect)
 */
int main(int argc, char **argv)
{
    // Variables
    // Stores the frames
    std::map <unsigned int, Frame<int> > frames = std::map <unsigned int, Frame<int> > ();
    // The input stream to grab data from
    std::shared_ptr<TextFileReader<int> > input = std::make_shared<TextFileReader<int> >();
    // The output stream for the log file
    std::ofstream log;


    // Handle the arguments passed into the program
    if (argc == 3) {
        // attempt to open and read the file specified
        try {
            // Set up the input strings for comparison later
            std::string mode = argv[1];
            std::string filePath = argv[2];
            std::string settings = "";

            // Open a log file
            log.open(LOG_FILE_NAME, std::fstream::out | std::fstream::binary);
            log << "Opened log file\n";


            log << "Opening detector dataset: " << filePath << "\n";
            input->open(filePath); // Open the input data file

            unsigned int frameNumber = 1; // Stores the current number of frames read

            log << "Starting frame retrieval loop...\n";
            while (!input->endOfStream()) {
                Frame<int> frame(input->getFrame());

                //logFrameDetails(log, frame, frameNumber);

                // Store the frame in the map of frames
                frames[frameNumber] = frame;
                // Increase the counter for the number of frames processed
                frameNumber++;
            }
            log << "Finished reading in data\n";
            
            log << "Number of frames is:\n "
                << (frameNumber - 1)
                << " frames\n";



            // Check the mode and do the correct actions
            // If on table generation mode:
            if (mode == "t" || mode == "-t")
            {
                // Create the table entry generator
                std::shared_ptr<TableEntryGen> tableEntryGen = std::make_shared<TableEntryGen>(
                    input->detectorName(),    // The name of the detector
                    input->size(),            // The size of the file in bytes
                    input->numberOfLines(),   // The number of lines in the file
                    frameNumber,              // The number of frames in the file
                    input->settings()         // The settings string for the data set
                );

                std::string entry = tableEntryGen->generateEntry();

                log << "Generated table entry:\n"
                    << entry << "\n";

                std::cout << entry << "\n";
            }
            // If on calibration mode:
            else if (mode == "c" || mode == "-c")
            {
                // TODO
            }


            // Clean-up
            log << "Closing input file\n";
            input->close();
            log << "Closing log file\n";
            log.close();

        } catch (std::ifstream::failure const& e) {
            std::cerr << "An error occurred: " << e.what() << "\n";

            // Close and exit on failure to open or read from the file
            input->close();
            log.close();

            std::exit(1);
        } catch (...) {
            log << "An unforeseen error has occurred!\n";

            // Close and exit on failure to open or read from the file
            log << "Closing input file\n";
            input->close();

            log << "Closing log file\n";
            log.close();

            std::exit(1);
        }
    } else { // If there are an incorrect number of arguments
        // Output an error message and a help message for usage
        std::cerr << "Error: Incorrect arguments were used!\n\n"
                << "USAGE: " << argv[0] << " mode input-cluster-log-name\n"
                << "mode\tThe mode to run in: \n\t'-t' for Wiki table entry generation,"
                << "\n\t'-c' for calibration mode\n" << std::endl;
    }

    return 0;
}
