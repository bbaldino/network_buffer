#pragma once

#include "network_buffer_view.hpp"

template<unsigned int BUF_SIZE = 1500>
class NetworkBuffer : public NetworkBufferView {
public:
    NetworkBuffer() :
        NetworkBufferView(m_buffer) {}
    size_t remainingCapacity() const {
        return BUF_SIZE - (m_tail - m_buffer);
    }

    /**
     * Return the position in the buffer to be written
     * to next
     */
    const uint8_t* const getBuffer() const {
        return m_head;
    }

    /**
     * Get a non-const version of the buffer (to be used, 
     * for example, when reading from the network)
     */
    uint8_t* getBuffer() {
        return m_head;
    }

//protected:
    uint8_t m_buffer[BUF_SIZE]; 
};
