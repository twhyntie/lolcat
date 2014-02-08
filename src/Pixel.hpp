/**
 * @file        Pixel.hpp
 * @author      Hector Stalker <hstalker0@gmail.com>
 * @version     0.1
 *
 * @brief       Defines the pixel class for storing pixels
 *
 * @copyright   This file is under the BSD 2-Clause license <br>
 * For conditions of distribution and use, see: <br>
 * http://opensource.org/licenses/BSD-2-Clause <br>
 * or read the 'LICENSE' file distributed with this code <br>
 */

#ifndef PIXEL_HPP
#define	PIXEL_HPP

// C++ headers
#include <ostream>
#include <cassert>

/**
 * @brief This class defines the pixel datatype used to store each pixel
 */
template <class T>
class Pixel {
public:

    /**
     * @brief   An empty constructor for the Pixel class
     * @return  A newly constructed Pixel object defaulted to 0 values
     */
    Pixel()
    : x_(0), y_(0), c_(0)
    {
    }

    /**
     * @brief   A constructor for the Pixel class
     * @param x The pixel's x position
     * @param y The pixel's y position
     * @param c The pixel's count value
     * @return  A newly constructed Pixel object
     */
    Pixel(T const x, T const y, T const c)
    : x_(x), y_(y), c_(c)
    {
    }

    /**
     * @brief       The copy constructor for the Pixel class
     * @param other The other pixel object to be copied from
     * @return      A new copy constructed Pixel object
     */
    Pixel(Pixel<T> const& other)
    : x_(other.x_), y_(other.y_), c_(other.c_)
    {
    }

    /**
     * @brief   The destructor for the Pixel class
     * @return  Nothing
     */
    ~Pixel()
    {
    }

    /**
     * @brief       The assignment operator for the Pixel class
     * @param other The other pixel object to be assigned from
     * @return      A pointer to this object
     */
    Pixel<T>& operator=(Pixel<T> other)
    {
        swap(*this, other);

        return *this;
    }

    /**
     * @brief       The equality operator - Checks equality
     * @param other The other Pixel object being compared against
     * @return      The result of the compare
     */
    bool operator==(Pixel<T> const& other)
    {
        // Compare the internal data
        if (other.x() == this->x_
                && other.y() == this->y_
                && other.c() == this->c_) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief       The inequality operator - Checks inequality
     * @param other The other Pixel object being compared against
     * @return      The result of the compare
     */
    bool operator!=(Pixel<T> const& other)
    {
        // Check if they aren't the same using the equality operator already defined
        if (!(this == other)) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief   Retrieves the x position of the pixel
     * @return  An value for the x position
     */
    T const x() const
    {
        return x_;
    }

    /**
     * @brief   Retrieves the y position of the pixel
     * @return  An value for the y position
     */
    T const y() const
    {
        return y_;
    }

    /**
     * @brief   Retrieves the count value of the pixel
     * @return  A number for the count value position
     */
    T const c() const
    {
        return c_;
    }

    /**
     * @brief   Retrieves ---
     * @return  A number for ---
     */
    T const xy() const
    {

        return static_cast<T> ((256 * y_) + x_);
    }

    template<class T2>
    friend void swap(Pixel<T2>&, Pixel<T2>&);

private:
    T x_, y_, c_;
};

/**
 * @brief        The swap operator for the Pixel object type
 * @param first  The first object to swap
 * @param second The second object to swap
 * @return       Nothing
 */
template <class T>
void swap(Pixel<T>& first, Pixel<T>& second) // no-throw
{
    using std::swap;
    swap(first.x_, second.x_);
    swap(first.y_, second.y_);
    swap(first.c_, second.c_);
}

/**
 * @brief        The global output stream operator - outputs pixel data to a stream
 * @param stream The output stream
 * @param pixel  The pixel being output
 * @return       A reference to the output stream
 */
template <class T>
std::ostream& operator<<(std::ostream& stream, Pixel <T> const& pixel)
{
    stream << "x = " << pixel.x() << std::endl
            << "y = " << pixel.y() << std::endl
            << "c = " << pixel.c() << std::endl
            << "xy = " << pixel.xy() << std::endl;
    return stream;
}


#endif	/* PIXEL_HPP */
