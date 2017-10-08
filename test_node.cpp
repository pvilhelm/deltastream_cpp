#include <cinttypes>

#include "catch.hpp"

#include "node.h"

using namespace dstream;

TEST_CASE("node.h") {
    
    SECTION("address") {
        Address a0((uint32_t)0x01020304, 1234);
        Address a1((uint32_t)0x01020305, 1234);
        Address a2((uint32_t)0x01020305, 1235);
        Address a22((uint32_t)0x01020305, 1235);
        /* Check < operator */
        REQUIRE(a0 < a1);
        REQUIRE(a1 < a2);

        /* Check > operator */
        REQUIRE(a1 > a0);
        REQUIRE(a2 > a1);

        /* Check == != operator */
        REQUIRE(a2 == a22);
        REQUIRE(a1 != a2);

        /* Check getPort() */
        REQUIRE(a0.getPort() == 1234);

        /* Check isIP4() */
        REQUIRE(a0.isIPv4() == true);

        /* Check getIP4() */
        REQUIRE(a0.getIPv4() == (uint32_t)0x01020304);

        /* Check getIP6() */
        IP ip6_0(0x01020304);
        REQUIRE(a0.getIP() == ip6_0);

        /* Same for IP6 address */

        uint64_t ip0[2] = {2,1}; /* LE! */
        Address a3(ip0, 1234);
        uint64_t ip1[2] = { 1,1 };
        Address a4(ip1, 1234);
        uint64_t ip3[2] = {1,0};
        Address a5(ip3, 1234);        
        Address a6(ip3, 1234);
        Address a7(ip3, 1233);
        /* == !=*/
        REQUIRE(a6 == a5);
        REQUIRE(a6 != a4);
        REQUIRE(a6 != a1);

        /* < > */
        REQUIRE(a7 < a6);
        REQUIRE(a4 < a3);
        REQUIRE(a2 < a3);  
        REQUIRE(a5 < a2);
        REQUIRE(a3 > a4);
        REQUIRE(a3 > a2);
        REQUIRE(a2 > a5);

        REQUIRE(a3.isIPv4() == false);
        REQUIRE(a3.getIPv4() == 0);
        REQUIRE(a3.getPort() == 1234);
        REQUIRE(a3.getIP() == IP(ip0));
    }
}