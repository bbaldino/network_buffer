#pragma once

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
     * Returns the amount of bytes written to the buffer
     * NOTE: this will include all bytes written and
     * will not exclude any bytes that may have been read
     * from the head of the buffer
     */
    size_t size() const {
        return _tail - _buffer;
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
