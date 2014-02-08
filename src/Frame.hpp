/**
 * @file        Frame.hpp
 * @author      Hector Stalker <hstalker0@gmail.com>
 * @version     0.1
 *
 * @brief       Defines the frame class for storing frame data and metadata
 *
 * @copyright   This file is under the BSD 2-Clause license <br>
 * For conditions of distribution and use, see: <br>
 * http://opensource.org/licenses/BSD-2-Clause <br>
 * or read the 'LICENSE' file distributed with this code <br>
 */

#ifndef FRAME_HPP
#define	FRAME_HPP

// C++ headers
#include <ostream>
#include <cassert>
#include <map>
// C++ headers
#include <Pixel.hpp>

/**
 * @brief This class defines the frame data-type for storing every frame and
 * their data with some meta-data
 */
template <class T>
class Frame {
public:

    /**
     * @brief   An empty constructor for the Frame class
     * @return  A newly constructed Frame object defaulted to null values
     */
    Frame()
    : pixels_(std::map<unsigned int, Pixel<T> >()), time_(0.0f), runningTime_(0.0f)
    {
    }

    /**
     * @brief An normal constructor for the Frame class
     * @param pixels The pixels data set to set the internal pixel map to
     * @param time The time in seconds to set the frame time to
     * @param runningTime The running time in seconds to set the frame's
     * running time to
     * @return  A newly constructed Frame object set to the given value
     */
    Frame(std::map<unsigned int, Pixel<T> > const& pixels,
            float const time,
            float const runningTime)
    : pixels_(pixels), time_(time), runningTime_(runningTime)
    {
    }

    /**
     * @brief       The copy constructor for the Frame class
     * @param other The other pixel object to be copied from
     * @return      A new copy constructed Frame object
     */
    Frame(Frame<T> const& other)
    : pixels_(other.pixels_),
    runningTime_(other.runningTime_),
    time_(other.time_)
    {
    }

    /**
     * @brief   The destructor for the Frame class
     * @return  Nothing
     */
    ~Frame()
    {
    }

    /**
     * @brief       The assignment operator for the Frame class
     * @param other The other pixel object to be assigned from
     * @return      A pointer to this object
     */
    Frame<T>& operator=(Frame<T>& other)
    {
        swap(*this, other);

        return *this;
    }

    /**
     * @brief     Retrieves the pixel specified by the key
     * @param key The pixel number to retrieve from the map
     * @return    A pixel object of the pixel specified, returns a defaulted
     * Pixel object if no appropriate pixel was found
     */
    Pixel<T> const getPixel(unsigned int const key) const
    {
        assert(!pixels_.empty());

        typename std::map<int, Pixel<T> >::iterator iter = pixels_.find(key);
        if (key != pixels_.end()) {
            return *iter;
        } else {
            std::cerr << "Couldn't find pixel: " << key << std::endl;

            return Pixel<T>();
        }

    }

    /**
     * @brief     Returns the pixel map
     * @return    A map of pixels
     */
    std::map<unsigned int, Pixel<T> > const getPixels() const
    {

        return pixels_;
    }

    /**
     * @brief     Retrieves the current time of the frame
     * @return    A floating point value with the current time in seconds
     */
    double const getTime() const
    {

        return time_;
    }

    /**
     * @brief     Retrieves the running time of the frame
     * @return    A floating point value with the running time in seconds
     */
    double const getRunningTime() const
    {

        return runningTime_;
    }

    /**
     * @brief       Sets the pixel specified by the key, and creates a new pixel
     * with that key if it doesn't exist already
     * @param key   The pixel number to set in the map
     * @param pixel The pixel data to add to the map
     * @return      Nothing
     */
    void setPixel(unsigned int const key, Pixel<T> const& pixel)
    {

        pixels_[key] = pixel;
    }

    /**
     * @brief      Retrieves the current time of the frame
     * @param time The time in seconds to set the time meta-data to
     * @return     Nothing
     */
    void setTime(float const time)
    {

        time_ = time;
    }

    /**
     * @brief             Sets the running time of the frame
     * @param runningTime The time in seconds to set the running time meta-data to
     * @return            Nothing
     */
    void setRunningTime(float const runningTime)
    {

        runningTime_ = runningTime;
    }

    template<class T2>
    friend void swap(Frame<T2>&, Frame<T2>&);

private:
    template<class T2>
    friend std::ostream& operator<<(std::ostream&, Frame <T2> const&);

    std::map<unsigned int, Pixel<T> > pixels_; // A map of the pixels in this frame
    double time_; // Stores the time since the 'Dawn of Time' in seconds
    double runningTime_; // Stores the time since the detector started running
};


/**
 * @brief        The swap operator for the Frame object type
 * @param first  The first object to swap
 * @param second The second object to swap
 * @return       Nothing
 */
template <class T>
void swap(Frame<T>& first, Frame<T>& second) // nothrow
{
    using std::swap;
    swap(first.pixels_, second.pixels_);
    swap(first.runningTime_, second.runningTime_);
    swap(first.time_, second.time_);
}


/**
 * @brief        The global output stream operator - outputs frame, and
 * thus pixel data, to a stream
 * @param stream The output stream
 * @param frame  The frame of which data is being output
 * @return       A reference to the output stream
 */
template<class T>
std::ostream& operator<<(std::ostream& stream, Frame <T> const& frame)
{
    // Print the meta-data
    stream << "C Time: " << frame.getTime() << std::endl
            << "Running Time: " << frame.getRunningTime() << std::endl;

    // Iterate over the pixels and print their data
    typename std::map< unsigned int, Pixel<T> >::const_iterator iter;
    for (iter = frame.pixels_.begin();
            iter != frame.pixels_.end();
            ++iter) {
        stream << "No. "
                << iter->first
                << " Pixel:\n"
                << iter->second;
    }

    return stream;
}


#endif	/* FRAME_HPP */
