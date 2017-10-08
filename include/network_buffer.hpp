#pragma once

#include <cstddef>

/**
 * Stores data in a buffer in network order, provides
 * convenience methods for writing to and reading
 * from the buffer
 */
class NetworkBuffer {
public:
    virtual ~NetworkBuffer() {}

    virtual void write(const uint8_t& val) = 0;

    virtual void write(const uint16_t& val) = 0;

    virtual void write(const uint32_t& val) = 0;

    /**
     * Directly write the contents of the given
     * buffer
     */
    virtual void write(const uint8_t* const buf, std::size_t numBytes) = 0;

    virtual uint8_t read8() = 0;

    virtual uint16_t read16() = 0;

    virtual uint32_t read32() = 0;

    /**
     * Directly read the contents of the buffer
     * Returns a pointer to the buffer at the
     * current point and advances the position
     * by the given number of bytes
     */
    virtual uint8_t* read(std::size_t numBytes) = 0;

    /**
     * Return the position in the buffer to be written
     * to next
     */
    virtual const uint8_t* const getBuffer() const = 0;

    /**
     * Get a non-const version of the buffer (to be used,
     * for example, when reading from the network)
     */
    virtual uint8_t* getBuffer() = 0;

    /**
     * Manually set the size of the buffer
     * NOTE: this should *only* be used when the internal
     * buffer has been grabbed (via getBuffer) and written
     * to directly.
     * TOOD: is there a better way to implement this?
     */
    virtual void setSize(std::size_t size) = 0;

    /**
     * Returns the current size of the buffer, which
     * is calculated as the amount of bytes written
     * minus any bytes read.
     */
    virtual size_t size() const = 0;

    virtual size_t remainingCapacity() const = 0;

    /**
     * Returns whether or not the buffer is
     * (effectively) empty.  Note that this
     * does not mean no bytes have been written,
     * it could mean that bytes were written and
     * then read
     */
    virtual bool empty() const = 0;

//protected:
};
