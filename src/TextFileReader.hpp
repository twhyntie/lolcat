/**
 * @file        TextFileReader.hpp
 * @author      Hector Stalker <hstalker0@gmail.com>
 * @version     0.1
 *
 * @brief       Defines the class for reading the pixel data
 * from the text encoded data file (class is non-copyable)
 *
 * @copyright   This file is under the BSD 2-Clause license <br>
 * For conditions of distribution and use, see: <br>
 * http://opensource.org/licenses/BSD-2-Clause <br>
 * or read the 'LICENSE' file distributed with this code <br>
 */

// TODO: Abstract away to provide an interface for parsing any kind of files easily

#ifndef TEXTFILEREADER_HPP
#define TEXTFILEREADER_HPP

// C++ headers
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <boost/filesystem.hpp>
// My headers
#include <Frame.hpp>

using namespace boost;

/**
 * @brief This class defines the class for handling reading of a text encoded data file
 * for pixel data (class is non-copyable)
 */
template <class T>
class TextFileReader {
public:

    /**
     * @brief   An empty constructor for the TextFileReader class
     * @return  A newly constructed TextFileReader object defaulted to null values
     */
    TextFileReader()
        : lineNumber_(1), currentPixelNumber_(1), detectorName_(""), fileSize_(0), numberOfLines_(0)
    {
    }


    /**
     * @brief      An constructor for the TextFileReader class
     * @param name The name of the data set's used detector to read from
     * @return     A newly constructed TextFileReader object
     */
    TextFileReader(std::string const& name)
        : lineNumber_(1), currentPixelNumber_(1), detectorName_(""), fileSize_(0), numberOfLines_(0)
    {
        this->open(name);
    }


    /**
     * @brief   The destructor for the TextFileReader class
     * @return  Nothing
     */
    ~TextFileReader()
    {
        if (in_.is_open()) {
            this->close();
        }
    }


    /**
     * @brief      A function to open the file stream
     * @param name The path of the data file to open
     * @return     Nothing
     */
    void open(std::string const& name)
    {
        if (!in_.is_open()) {
            filesystem::path filePath (name);

            if (filesystem::exists(filePath)) {
                if (filesystem::is_regular_file(filePath) && !filesystem::is_empty(filePath)) {
                    
                    // Grab the file size
                    fileSize_ = filesystem::file_size(filePath);

                    // Grab the detector name
                    detectorName_ = filePath.parent_path().parent_path().parent_path().leaf().string();

                    // Grab the settings string
                    settings_ = filePath.parent_path().leaf().string();

                    // Open the cluster log
                    try {
                        // Set the input file stream's exceptions flags
                        in_.exceptions(std::ifstream::failbit | std::ifstream::badbit);

                        // Try to open the file
                        in_.open(filePath.string(), std::ifstream::in | std::ifstream::binary);
                        lineNumber_ = 1;
                    }
                    catch (std::ifstream::failure& e) {
                        // Catch an error when attempting to open a file

                        std::cerr << "An error occurred when opening the file!\n"
                            << e.what() << std::endl;
                        throw;
                    }

                    // Reset the exception flags to allow the getline loop
                    in_.exceptions(std::ifstream::goodbit);

                    std::string throwaway;
                    // Get the number of lines in the file
                    // NOTE: if reading from disk is a bottleneck, we could cache every line here to memory
                    while (std::getline(in_, throwaway)) {
                        ++numberOfLines_;
                    }
                    // Reset the file state
                    in_.clear();
                    in_.seekg(0, std::ios::beg);

                    // Reset the exception flags to allow exceptions from now on
                    in_.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                    
                }
                else {
                    std::cerr << "An error occurred when opening the file!\n"
                              << "Detector name '" << filePath.string() << "' isn't a regular file or is empty!" << std::endl;
                    std::exit(1);
                }
            }
            else {
                std::cerr << "An error occurred when opening the file!\n"
                          << "Data file '" << filePath.string() << "' doesn't exist!" << std::endl;
                std::exit(1);
            }
        }
    }


    /**
     * @brief      A function to close the file stream
     * @return     Nothing
     */
    void close()
    {
        if (in_.is_open()) {
            detectorName_ = "";
            in_.close();
        }
    }


    /**
     * @brief      A function to check whether the end of the stream has been
     * reached
     * @return     A boolean stating whether the stream's end has been reached
     */
    bool endOfStream()
    {
        in_.peek(); // Peek to force the EOF flag to be set
        if (!in_.eof()) {

            return false;
        }

        return true;
    }


    /**
     * @brief      A function to close the file stream
     * @return     Returns a Frame object with the data from the frame being
     * read.
     */
    Frame<T> const getFrame()
    {
        try {
            Frame<T> frame = Frame<T> (); // Construct a new frame
            // Reset the current pixel counter to 0
            currentPixelNumber_ = 0;
            // Flag as to whether the data section of a frame has begun to be read
            bool isInFrame = false;
            // Flags for checking the result of an attempted parse on a line
            bool isMetadata = false;
            bool isClusterdata = false;


            // While it isn't the end of the file yet
            while (!endOfStream()) {
                // Parse the data

                // If a frame hasn't been begun
                if (!isInFrame) {
                    // Try to read the meta-data string from the file
                    // isMetadata will be false if the string is not a meta-data string
                    isMetadata = parseMetadataString(frame);
                    if (!isMetadata) {
                        // If there was no meta-data for the first line of the frame
                        // then report an error
                        std::ostringstream o;
                        o << "Malformed data file: Missing meta-data string at line: "
                          << lineNumber_;
                        throw std::ifstream::failure(o.str());
                    }

                    // Flag that a frame has been begun
                    isInFrame = true;
                } else {
                    // If the meta-data of the frame has already been read
                    // then read the cluster data into pixel storage

                    // First try to read a line of cluster data
                    isClusterdata = parseClusterString(frame);
                    // If it isn't cluster data then use the empty line
                    if (!isClusterdata) {
                        eatLine();
                        // then end operations for this frame
                        break;
                    }
                }
            }

            return frame;
        } catch (std::ifstream::failure const& e) {

            std::cerr << "An error occurred when reading a line from the file!\n"
                    << e.what() << std::endl;
            throw; // Re-throw the exception up the stack
        }
    }


    /**
    * @brief      A getter for the detector used to generate the data's name
    * @return     Returns a string containing the detector's name
    */
    std::string const detectorName()
    {
        return detectorName_;
    }


    /**
    * @brief      A getter for the settings used to generate the data
    * @return     Returns a string containing the settings
    */
    std::string const settings()
    {
        return settings_;
    }


    /**
    * @brief      A getter for the number of lines in the file
    * @return     Returns a an integer for the number of lines in the file
    */
    unsigned int const numberOfLines()
    {
        return numberOfLines_;
    }


    /**
    * @brief      A getter for the file size
    * @return     Returns a an integer for the size of the file in bytes
    */
    unsigned int const size()
    {
        return fileSize_;
    }

private:

    // Non-copyable
    // Copy constructor
    TextFileReader(TextFileReader const& other)
    {
    }


    // Assignment operator
    TextFileReader<T>& operator=(TextFileReader<T>& other)
    {
        return *this;
    }


    // Utility Functions
    void eatLine()
    {
        assert(in_.is_open());

        // Simply eats up the current line without doing anything

        if (!endOfStream()) {

            std::string line; //A temporary for lines

            // Read in a line
            std::getline(in_, line);

            lineNumber_++;
        }
    }


   std::string const extractSubstring(
        std::string const& input,
        size_t& currentPosition,
        char const delimeter
    ) const
    {
        // Iterates the given string from currentPosition until delimeter is found,
        // and returns the string from the given position up to the position to the
        // delimeter (which is excluded)), also changes the value of the
        // currentPosition variable

        std::string temp = "";
        while (currentPosition < input.length() && input[currentPosition] != ' ') {

            temp += input[currentPosition]; // Add the characters to the temporary
            currentPosition++; // Increment the input position
        }

        return temp;
    }


    // Parsing routines
    bool const parseMetadataString(Frame<T>& frame)
    {
        assert(in_.is_open());

        // Attempts to parse and extract the fields into the frame object's data
        // fields from a meta-data string of this format - e.g.
        // 'Frame 1 (1335967757.2905033 s, 0.1 s)'
        // Returns false if the string is not meta-data, and thus resets the
        // file pointer

        if (!endOfStream()) {
            // Store the state of the file pointer
            size_t originalFilePosition = in_.tellg();

            //--------------------------------
            // Get to the beginning of the parentheses
            std::string line; //A temporary for lines

            // Read in a line
            std::getline(in_, line);

            // find the opening parenthesis position in the string
            size_t pos = line.find("(");
            // If the opening parenthesis is not present, then this is not a
            // meta-data string
            if (pos == std::string::npos) {
                // Reset the state of the file pointer
                in_.seekg(originalFilePosition);
                // And return a false value to state that this isn't meta-data
                return false;
            }

            //--------------------------------
            // Get the time meta-data
            // Move to the first digit of the C-time
            pos++;

            // Grab and store the real time in the current frame object
            frame.setTime(atof(extractSubstring(line, pos, ' ').c_str()));
            // Skip the number of characters necessary to reach the position
            // of the beginning of the running time string
            pos += std::string(" s, ").length();

            // Extract the running time string
            frame.setRunningTime(atof(extractSubstring(line, pos, ' ').c_str()));

            lineNumber_++; // Keep track of the current line number

            return true;
        } else { // If the file has ended

            return false;
        }
    }


    bool const parseClusterString(Frame<T>& frame)
    {
        assert(in_.is_open());

        // Attempts to parse and extract the fields into the frame object's data
        // fields from a meta-data string of this format - e.g.
        // '[19, 0, 55]' where x = 19, y = 0, c = 55
        // Returns false if the string is not a cluster string, and thus resets
        // the file pointer

        if (!endOfStream()) {
            // Store the state of the file pointer
            size_t originalFilePosition = in_.tellg();

            //--------------------------------
            // Get to the beginning of the parentheses
            std::string line; //A temporary for lines

            // Read in a line
            std::getline(in_, line);

            // find the opening square bracket position in the string
            size_t pos = line.find("[");
            // If the opening parenthesis is not present, then this is not a
            // cluster data string
            if (pos == std::string::npos) {
                // Reset the state of the file pointer
                in_.seekg(originalFilePosition);
                // And return a false value to state this isn't cluster data
                return false;
            }

            //--------------------------------
            // Get the cluster data
            // Create the data field variables
            T x = 0;
            T y = 0;
            T c = 0;

            // Move to the first digit of the x value
            pos++;

            // Grab and store the x position value
            x = atoi(extractSubstring(line, pos, ',').c_str());
            // Skip the number of characters necessary to reach the position
            // of the beginning of the y value string
            pos += std::string(", ").length();

            // Grab and store the y position value
            y = atoi(extractSubstring(line, pos, ',').c_str());
            // Skip the number of characters necessary to reach the position
            // of the beginning of the c value string
            pos += std::string(", ").length();

            // Grab and store the count value
            c = atoi(extractSubstring(line, pos, ',').c_str());

            // Construct a pixel object with this data and add it to the frame
            frame.setPixel(
                    ++currentPixelNumber_,
                    Pixel<T>(x, y, c)
                    );


            lineNumber_++;
            return true;
        } else { // If the file has ended

            return false;
        }
    }

    std::ifstream in_; // The input stream for data
    std::string detectorName_; // The input's file name
    std::string settings_; // The settings used when generating the data
    unsigned int currentPixelNumber_; // The number of the next pixel to be stored
    unsigned int lineNumber_; // The current line number in the file
    unsigned int numberOfLines_; // The total number of lines in the file
    unsigned int fileSize_; // The size of the file in bytes
};


#endif  /* TEXTFILEREADER_HPP */
