#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "network_buffer.hpp"

#include <iostream>

using namespace std;

void doInitializationTests(NetworkBufferView& buffer) {
    REQUIRE(buffer.empty() == true);
    REQUIRE(buffer.size() == 0);
}

void doWriteTests(NetworkBufferView& buffer) {
    SECTION("uint8_t") {
        uint8_t writeVal = 42;
        buffer.write(writeVal);
        uint8_t bufferVal;
        buffer.readInto(&bufferVal, sizeof(uint8_t));
        REQUIRE(bufferVal == writeVal);
    }

    SECTION("uint16_t") {
        uint16_t writeVal = 0xDEAD;
        buffer.write(writeVal);
        uint16_t bufferVal;
        buffer.readInto(reinterpret_cast<uint8_t*>(&bufferVal), sizeof(uint16_t));
        REQUIRE(bufferVal == htons(writeVal));
    }

    SECTION("uint32_t") {
        uint32_t writeVal = 0xDEADBEEF;
        buffer.write(writeVal);
        uint32_t bufferVal;
        buffer.readInto(reinterpret_cast<uint8_t*>(&bufferVal), sizeof(uint32_t));
        REQUIRE(bufferVal == htonl(writeVal));
    }

    SECTION("buffer") {
        uint8_t writeVal[4] = { 0xDE, 0xAD, 0xBE, 0XEF };
        buffer.write(writeVal, 4);
        uint8_t bufferVal[4];
        buffer.readInto(bufferVal, 4);
        for (auto i = 0; i < 4; ++i) {
            REQUIRE(bufferVal[i] == writeVal[i]);
        }
    }
}

void doReadTests(NetworkBufferView& buffer) {
    // We've already verified write, so we can use write
    // here and assume it works

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

    SECTION("buffer") {
        uint8_t writeVal[4] = { 0xDE, 0xAD, 0xBE, 0XEF };
        buffer.write(writeVal, 4);
        uint8_t readVal[4];
        buffer.readInto(readVal, 4);
        for (auto i = 0; i < 4; ++i) {
            REQUIRE(readVal[i] == writeVal[i]);
        }
    }
}

void doSizeTests(NetworkBufferView& buffer) {
    SECTION("Size increases after writes") {
        REQUIRE(buffer.empty() == true);
        REQUIRE(buffer.size() == 0);
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
        REQUIRE(buffer.empty() == false);
    }

    SECTION("Size decreases after reads") {
        buffer.write(static_cast<uint8_t>(42));
        buffer.write(static_cast<uint8_t>(42));
        buffer.write(static_cast<uint16_t>(512));
        buffer.write(static_cast<uint32_t>(512 * 512));
        buffer.write(static_cast<uint16_t>(512));
        buffer.write(static_cast<uint8_t>(42));

        buffer.read8();
        REQUIRE(buffer.size() == 10);
        buffer.read8();
        REQUIRE(buffer.size() == 9);

        buffer.read16();
        REQUIRE(buffer.size() == 7);

        buffer.read32();
        REQUIRE(buffer.size() == 3);

        buffer.read16();
        REQUIRE(buffer.size() == 1);

        buffer.read8();
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.empty() == true);
    }
}

TEST_CASE("Bring your own buffer") {
    uint8_t backingBuffer[1500];
    NetworkBufferView buffer(backingBuffer);

    doInitializationTests(buffer);
    doWriteTests(buffer);
    doReadTests(buffer);
    doSizeTests(buffer);
}

TEST_CASE("NetworkBuffer") {
    NetworkBuffer<> buffer;

    doInitializationTests(buffer);
    doWriteTests(buffer);
    doReadTests(buffer);
    doSizeTests(buffer);
}
