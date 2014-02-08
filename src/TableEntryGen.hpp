/**
 * @file        TableEntryGen.hpp
 * @author      Hector Stalker <hstalker0@gmail.com>
 * @version     0.1
 *
 * @brief       Defines the class for generating the Wiki table entry
 *              for the cataloging of the input data from being given
 *              the details
 *
 * @copyright   This file is under the BSD 2-Clause license <br>
 * For conditions of distribution and use, see: <br>
 * http://opensource.org/licenses/BSD-2-Clause <br>
 * or read the 'LICENSE' file distributed with this code <br>
 */

#ifndef TABLEENTRYGEN_HPP
#define TABLEENTRYGEN_HPP

// C++ headers
#include <string>


/**
 * @brief This class is for generating the Wiki table entry for the metadata of the dataset
 */
class TableEntryGen {
public:

    /**
     * @brief A constructor for the TableEntryGen class
     * @param detectorName The name of the detector used to create the dataset
     * @param fileName The name of the cluster log for the dataset
     * @param fileSize The size of the cluster log file in bytes
     * @param fileNumberOfLines The number of lines in the cluster log file
     * @param fileNumberOfFrames The number of frames in the cluster log file
     * @param chipSettings
     * @return A newly constructed TableEntryGen object
     */
    TableEntryGen(
        std::string const& detectorName,
        unsigned int const fileSize,
        unsigned int const fileNumberOfLines,
        unsigned int const fileNumberOfFrames,
        std::string chipSettings
    )
        : detectorName_(detectorName), size_(fileSize), numberOfLines_(fileNumberOfLines),
        numberOfFrames_(fileNumberOfFrames), settings_(chipSettings)
    {
    }


    /**
     * @brief   The destructor for the TableEntryGen class
     * @return  Nothing
     */
    ~TableEntryGen()
    {
    }


    /**
     * @brief      Generates the table entry with the details from the input info
     * @return     Returns a string containing the formatted table entry
     */
    std::string const generateEntry();

private:
    // Non-copyable
    // The copy constructor
    TableEntryGen(TableEntryGen const& other)
    {
    }


    // Assignment operator
    TableEntryGen& operator=(TableEntryGen& other)
    {
    }


    // The storage variables for the metadata of the data file
    std::string detectorName_;
    std::string settings_;
    unsigned int size_;
    unsigned int numberOfLines_;
    unsigned int numberOfFrames_;
};


#endif  /* TABLEENTRYGEN_HPP */
