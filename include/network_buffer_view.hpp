#pragma once

class NetworkBufferView {
public:
    NetworkBufferView(uint8_t* const buf) :
        m_buf(buf), m_head(buf), m_tail(buf) {}

    void write(const uint8_t& val) {
        doWrite(val);
    }

    void write(const uint16_t& val) {
        uint16_t networkVal = htons(val);
        doWrite(networkVal);
    }

    void write(const uint32_t& val) {
        uint32_t networkVal = htonl(val);
        doWrite(networkVal);
    }

    /**
     * Write the contents of the given
     * buffer
     */
    void write(const uint8_t* const buf, std::size_t numBytes) {
        memcpy(m_tail, buf, numBytes);
        m_tail += numBytes;
    }

    uint8_t read8() {
        return doRead<uint8_t>();
    }

    uint16_t read16() {
        uint16_t res = doRead<uint16_t>();
        return ntohs(res);
    }

    uint32_t read32() {
        uint32_t res = doRead<uint32_t>();
        return ntohl(res);
    }

    /**
     * Read numBytes from this buffer into the given
     * buffer
     */
    void readInto(uint8_t* const buf, std::size_t numBytes) {
        memcpy(buf, m_head, numBytes);
        m_head += numBytes;
    }

    void seek(std::size_t numBytes) {
        m_head += numBytes;
    }

    /**
     * Returns the current size of the buffer, which
     * is calculated as the amount of bytes written
     * minus any bytes read.
     */
    size_t size() const {
        return m_tail - m_head;
    }

    /**
     * Returns whether or not the buffer is
     * (effectively) empty.  Note that this
     * does not mean no bytes have been written,
     * it could mean that bytes were written and
     * then read
     */
    bool empty() const {
        return m_tail == m_head;
    }

//protected:
    uint8_t* const m_buf = nullptr;
    uint8_t* m_head = nullptr; // Tracks reading
    uint8_t* m_tail = nullptr; // Tracks writing

    template<typename T>
    void doWrite(const T& val) {
        memcpy(m_tail, &val, sizeof(T));
        m_tail += sizeof(T);
    }

    template<typename T>
    T doRead() {
        T val;
        memcpy(&val, m_head, sizeof(T));
        m_head += sizeof(T);
        return val;
    }
};
