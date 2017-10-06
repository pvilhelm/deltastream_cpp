#include <cinttypes>

#include "catch.hpp"

#include "node.h"

using namespace dstream;

TEST_CASE("node.h") {
    
    SECTION("address") {
        address a0((uint32_t)0x01020304, 1234);
        address a1((uint32_t)0x01020305, 1234);
        address a2((uint32_t)0x01020305, 1235);

        /* Check < operator */
        REQUIRE(a0 < a1);
        REQUIRE(a1 < a2);

        /* Check getPort() */
        REQUIRE(a0.getPort() == 1234);

        /* Check isIP4() */
        REQUIRE(a0.isIP4() == true);

        /* Check getIP4() */
        REQUIRE(a0.getIP4() == (uint32_t)0x01020304);

        /* Check getIP6() */
        ip6 ip6_0;
        REQUIRE(a0.getIP6() == ip6_0);

        /* SAme for IP6 address */

        uint64_t ip0[2] = {2,1}; /* LE! */
        address a3(ip0, 1234);
        uint64_t ip1[2] = { 1,1 };
        address a4(ip1, 1234);
        REQUIRE(a4 < a3);
        REQUIRE(a2 < a3); /* IP4 allways smaller */

        REQUIRE(a3.isIP4() == false);
        REQUIRE(a3.getIP4() == 0);
        REQUIRE(a3.getPort() == 1234);
        REQUIRE(a3.getIP6() == ip6(ip0));
    }
}