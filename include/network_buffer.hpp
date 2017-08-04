#pragma once

#include <cstddef>
#include <cassert>

/**
 * Stores data in a buffer in network order, provides
 * convenience methods for writing to and reading
 * from the buffer
 */
template<unsigned int BUF_SIZE = 1500>
class NetworkBuffer {
public:
    NetworkBuffer() :
        _head(_buffer), _tail(_buffer) {}

    void write(const uint8_t& val) {
        _write(val);
    }

    void write(const uint16_t& val) {
        uint16_t networkVal = htons(val);
        _write(networkVal);
    }

    void write(const uint32_t& val) {
        uint32_t networkVal = htonl(val);
        _write(networkVal);
    }

    /**
     * Directly write the contents of the given
     * buffer
     */
    void write(const uint8_t* const buf, std::size_t numBytes) {
        assert(_tail + numBytes <= _buffer + BUF_SIZE);
        memcpy(_tail, buf, numBytes);
        _tail += numBytes;
    }

    uint8_t read8() const {
        return _read<uint8_t>();
    }

    uint16_t read16() const {
        uint16_t res = _read<uint16_t>();
        return ntohs(res);
    }

    uint32_t read32() const {
        uint32_t res = _read<uint32_t>();
        return ntohl(res);
    }

    /**
     * Directly read the contents of the buffer
     * Returns a pointer to the buffer at the
     * current point and advances the position
     * by the given number of bytes
     */
    uint8_t* read(std::size_t numBytes) const {
        assert(_head + numBytes <= _buffer + BUF_SIZE);
        uint8_t* currPos = _head;
        _head += numBytes;
        return currPos;
    }

    const uint8_t* const getBuffer() const {
        return _buffer;
    }

    /**
     * Get a non-const version of the buffer (to be used, 
     * for example, when reading from the network)
     */
    uint8_t* getBuffer() {
        return _buffer;
    }

    /**
     * Manually set the size of the buffer
     * NOTE: this should *only* be used when the internal
     * buffer has been grabbed (via getBuffer) and written
     * to directly.
     * TOOD: is there a better way to implement this?
     */
    void setSize(std::size_t size) {
        assert(size <= BUF_SIZE);
        _tail += size;
    }

    /**
     * Returns the current size of the buffer, which
     * is calculated as the amount of bytes written
     * minus any bytes read.
     */
    size_t size() const {
        return _tail - _head;
    }

    /**
     * Returns whether or not the buffer is
     * (effectively) empty.  Note that this
     * does not mean no bytes have been written,
     * it could mean that bytes were written and
     * then read
     */
    bool empty() const {
        return _tail == _head;
    }

//protected:
    uint8_t _buffer[BUF_SIZE]; 
    mutable uint8_t* _head; // Tracks reading
    uint8_t* _tail; // Tracks writing

    template<typename T>
    void _write(const T& val) {
        assert(_tail + sizeof(T) < _buffer + BUF_SIZE);
        memcpy(_tail, &val, sizeof(T));
        _tail += sizeof(T);
    }

    template<typename T>
    T _read() const {
        assert(_head < (_tail + sizeof(T)));
        T val;
        memcpy(&val, _head, sizeof(T));
        _head += sizeof(T);
        return val;
    }
};
