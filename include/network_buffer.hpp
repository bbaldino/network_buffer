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

//protected:
    uint8_t m_buffer[BUF_SIZE]; 
};
