#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "network_buffer.hpp"

#include <iostream>

using namespace std;

TEST_CASE("Initialization") {
    NetworkBuffer<> buffer;
    REQUIRE((void*)buffer.getBuffer() == (void*)buffer.read(0));
}

TEST_CASE("Basic read/write tests") {
    NetworkBuffer<> buffer;

    SECTION("uint8_t") {
        uint8_t writeVal = 42;
        buffer.write(writeVal);
        uint8_t readVal = buffer.read8();
        REQUIRE(readVal == writeVal);
    }

    SECTION("uint16_t") {
        uint16_t writeVal = 0xDEAD;
        buffer.write(writeVal);
        uint16_t readVal = buffer.read16();
        REQUIRE(readVal == writeVal);
    }

    SECTION("uint32_t") {
        uint32_t writeVal = 0xDEADBEEF;
        buffer.write(writeVal);
        uint32_t readVal = buffer.read32();
        REQUIRE(readVal == writeVal);
    }
}

TEST_CASE("Size tests") {
    NetworkBuffer<> buffer;

    buffer.write(static_cast<uint8_t>(42));
    REQUIRE(buffer.size() == 1);
    buffer.write(static_cast<uint8_t>(42));
    REQUIRE(buffer.size() == 2);
    buffer.write(static_cast<uint16_t>(512));
    REQUIRE(buffer.size() == 4);
    buffer.write(static_cast<uint32_t>(512 * 512));
    REQUIRE(buffer.size() == 8);
    buffer.write(static_cast<uint16_t>(512));
    REQUIRE(buffer.size() == 10);
    buffer.write(static_cast<uint8_t>(42));
    REQUIRE(buffer.size() == 11);
}

TEST_CASE("Network order") {
    // Verify that the buffer is storing the data
    // in network order
    NetworkBuffer<> buffer;

    buffer.write(static_cast<uint8_t>(42));
    buffer.write(static_cast<uint16_t>(512));
    buffer.write(static_cast<uint32_t>(512 * 512));
    // Get the raw buffer and directly memcpy out of it
    const uint8_t* const buf = buffer.getBuffer();
    uint8_t bufOffset = 0;

    uint8_t networkByte;
    memcpy(&networkByte, buf + bufOffset, sizeof(uint8_t));
    bufOffset += sizeof(uint8_t);
    REQUIRE(networkByte == 42);

    uint16_t networkShort;
    memcpy(&networkShort, buf + bufOffset, sizeof(uint16_t));
    bufOffset += sizeof(uint16_t);
    REQUIRE(networkShort == htons(512));

    uint32_t networkLong;
    memcpy(&networkLong, buf + bufOffset, sizeof(uint32_t));
    REQUIRE(networkLong == htonl(512 * 512));
}

TEST_CASE("Read directly into buffer") {
    // Make sure that when we read into the buffer directly, we still
    // get the proper values out
    uint8_t networkByte = 42;
    uint16_t networkShort = htons(512);
    uint32_t networkLong = htonl(512 * 512);

    NetworkBuffer<> buffer;
    uint8_t* buf = buffer.getBuffer();
    memcpy(buf, &networkByte, sizeof(networkByte));
    buf += sizeof(networkByte);
    memcpy(buf, &networkShort, sizeof(networkShort));
    buf += sizeof(networkShort);
    memcpy(buf, &networkLong, sizeof(networkLong));
    buf += sizeof(networkLong);
    buffer.setSize(buf - buffer.getBuffer());

    REQUIRE(buffer.size() == sizeof(networkByte) + sizeof(networkShort) + sizeof(networkLong));
    uint8_t hostByte = buffer.read8();
    REQUIRE(hostByte == 42);
    uint16_t hostShort = buffer.read16();
    REQUIRE(hostShort == 512);
    uint32_t hostLong = buffer.read32();
    REQUIRE(hostLong == 512 * 512);
}

TEST_CASE("Direct read/write") {
    NetworkBuffer<> buffer;

    SECTION("string") {
        string str{"Hello, world"};
        buffer.write(reinterpret_cast<const uint8_t*>(str.c_str()), str.length());

        uint8_t* rawStr = buffer.read(str.length());
        string bufStr((char*)rawStr, str.length());

        REQUIRE(str.compare(std::string{bufStr}) == 0);
    }

    SECTION("data") {
        uint8_t buf[12] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};
        buffer.write(buf, 12);

        uint8_t* raw = buffer.read(12);

        for (auto i = 0; i < 12; ++i) {
            REQUIRE(raw[i] == buf[i]);
        }
    }
}
