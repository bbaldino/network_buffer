#include "catch.hpp"

#include "network_buffer_view.hpp"

#include "array_backed_network_buffer.hpp"

using namespace std;

TEST_CASE("NetworkBufferView") {

    SECTION("2 NetworkBufferViews from the same buf should give the same results") {
        ArrayBackedNetworkBuffer<> buf;
        buf.write(static_cast<uint8_t>(42));
        buf.write(static_cast<uint8_t>(24));

        NetworkBufferView bufView(buf);
        uint8_t readVal = bufView.read8();
        REQUIRE(readVal == static_cast<uint8_t>(42));
        readVal = bufView.read8();
        REQUIRE(readVal == static_cast<uint8_t>(24));

        NetworkBufferView bufView2(buf);
        readVal = bufView2.read8();
        REQUIRE(readVal == static_cast<uint8_t>(42));
        readVal = bufView2.read8();
        REQUIRE(readVal == static_cast<uint8_t>(24));
    }






}
