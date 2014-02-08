/**
 * @file        TableEntryGen.cpp
 * @author      Hector Stalker <hstalker0@gmail.com>
 * @version     0.1
 *
 * @brief       Defines the definition of the class for generating the Wiki table entry
 *              for the cataloging of the input data from being given
 *              the details
 *
 * @copyright   This file is under the BSD 2-Clause license <br>
 * For conditions of distribution and use, see: <br>
 * http://opensource.org/licenses/BSD-2-Clause <br>
 * or read the 'LICENSE' file distributed with this code <br>
 */

#ifndef TABLEENTRYGEN_CPP
#define TABLEENTRYGEN_CPP

// C++ headers
#include <string>
#include <iostream>
#include <sstream>
// My headers
#include <TableEntryGen.hpp>


std::string const TableEntryGen::generateEntry()
{
    std::stringstream entry;
    entry << "|-\n| " << detectorName_ << " || "
        << size_ << " || "
        << numberOfLines_ << " || "
        << numberOfFrames_ << " || "
        << settings_ << std::endl;

    return entry.str();
}


#endif  /* TABLEENTRYGEN_CPP */
