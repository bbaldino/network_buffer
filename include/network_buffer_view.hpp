#pragma once

#include <netinet/in.h>

#include "network_buffer.hpp"

/**
 * Read-only view of an existing buffer,
 * which allows advancing the positions
 * to read without affecting the buffer
 * it was constructed from
 */
//TODO: a lot of duplicate code between
//this and ArrayBackedNetworkBuffer, find
//a way to get rid of the dupes
class NetworkBufferView {
public:
    NetworkBufferView(const NetworkBuffer& buffer) :
        _head(buffer.data()) {}
    virtual ~NetworkBufferView() {}

    uint8_t read8() {
        return _read<uint8_t>();
    }

    uint16_t read16() {
        uint16_t res = _read<uint16_t>();
        return ntohs(res);
    }

    uint32_t read32() {
        uint32_t res = _read<uint32_t>();
        return ntohl(res);
    }

    const uint8_t* read(std::size_t numBytes) {
        const uint8_t* currPos = _head;
        _head += numBytes;
        return currPos;
    }
//protected:
    const uint8_t* _head;

    template<typename T>
    T _read() {
        T val;
        memcpy(&val, _head, sizeof(T));
        _head += sizeof(T);
        return val;
    }
};
