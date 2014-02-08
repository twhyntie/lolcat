###############################################################################
## ~Langtons' Own LUCID Calibration Analysis for Timepix (LOLCAT) CMakeLists~
###############################################################################
# Copyright (c) 2014, Hector Stalker
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
cmake_minimum_required(VERSION 2.8.3)
project(lolcat)

################################
## -Default build type-
################################
set(CMAKE_BUILD_TYPE Debug)


################################
## -Compiler flags-
################################
# Setting debug C++ flags for various platforms
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # If clang is being used
    message("Setting CLANG debug flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -O0 -Wall")

    message("Setting CLANG release flags")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -Wall")
    set(CMAKE_CXX_LINK_FLAGS_RELEASE "${CMAKE_CXX_LINK_FLAGS_RELEASE} -strip-all")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # If a gcc variant is being used
    message("Setting GCC-G++ debug flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -O0")

    message("Setting GCC-G++ release flags")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -s -Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # If intel C++ is being used
    message("Setting INTEL C++ debug flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  /Qstd=c++11")

    message("Setting INTEL C++ release flags")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  /Qstd=c++11")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # If MSVC is being used
    message("Setting MSVS C++ debug flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  /ZI /Od")
    set(CMAKE_CXX_LINK_FLAGS_DEBUG "${CMAKE_CXX_LINK_FLAGS_DEBUG} /DEBUG")

    message("Setting MSVS C++ release flags")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  /O2")
endif()
message("Debug flags set: ${CMAKE_CXX_FLAGS_DEBUG}\n${CMAKE_CXX_LINK_FLAGS_DEBUG}")
message("Release flags set: ${CMAKE_CXX_FLAGS_RELEASE}\n${CMAKE_CXX_LINK_FLAGS_RELEASE}")


################################
## -Code directories/files-
################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# find the source code files
set(SOURCE_FILES
    src/TableEntryGen.cpp
    src/Main.cpp
)


# Try to build the documentation
# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cfg ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc #ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)


################################
## -build commands-
################################
###############################################################################
# Boost Library
###############################################################################
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 
find_package(Boost REQUIRED COMPONENTS filesystem system) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS} ./src) 
    # Compile the main program
    add_executable(lolcat ${SOURCE_FILES})
    target_link_libraries(lolcat ${Boost_LIBRARIES})                                                                                                                                                                                                                            
endif()
