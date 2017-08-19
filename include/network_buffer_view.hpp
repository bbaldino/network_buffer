#pragma once

class NetworkBufferView {
public:
    NetworkBufferView(uint8_t* buf) :
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

    //TODO: from here down: still needed?
    /**
     * Return the position in the buffer to be written
     * to next
     */
    //const uint8_t* const getBuffer() const {
    //    return m_head;
    //}
    ///**
    // * Get a non-const version of the buffer (to be used, 
    // * for example, when reading from the network)
    // */
    //uint8_t* getBuffer() {
    //    return m_head;
    //}
    ///**
    // * Manually set the size of the buffer
    // * NOTE: this should *only* be used when the internal
    // * buffer has been grabbed (via getBuffer) and written
    // * to directly.
    // * TOOD: is there a better way to implement this?
    // */
    //void setSize(std::size_t size) {
    //    m_tail += size;
    //}

    ////DEPRECATED
    //uint8_t* read(std::size_t numBytes) {
    //    uint8_t* currPos = m_head;
    //    m_head += numBytes;
    //    return currPos;
    //}

//protected:
    uint8_t* m_buf = nullptr;
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
