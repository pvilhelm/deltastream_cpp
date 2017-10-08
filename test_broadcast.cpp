#include <cinttypes>
#include <memory>

#include "catch.hpp"

#include "node.h"
#include "broadcast.h"

using namespace dstream;

TEST_CASE("broadcast.h")
{
    SECTION("putNode() getNode() deleteNode() hasNode()")
    {
        Broadcast b;
        auto nptr = std::make_shared<Node>();
        nptr->setAddress(IP(0xC0A8010A),1111);
        b.putNode(nptr);
        Address a = nptr->getAddress();
        /* See if it exists */
        REQUIRE(b.hasNode(a));
        /* Get it */
        REQUIRE(nptr == b.getNode(a));

        /* Delete it */
        REQUIRE(b.deleteNode(a) == nptr);
        REQUIRE_THROWS(b.deleteNode(a)); /* Throws if it no node */
        REQUIRE_THROWS(b.getNode(a));
        REQUIRE_FALSE(b.hasNode(a));
    }
}