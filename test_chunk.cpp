#include <cinttypes>

#include "catch.hpp"

#include "chunk.h"

using namespace dstream;



TEST_CASE("chunk.h")
{
    SECTION("(de)serialize()")
    {
        Chunk c1(1, 1, 10, UDP_DATA, std::vector<uint8_t>{0,1,2,3});

        auto v1 = c1.serialize();
        Chunk c2 = Chunk::deserialize(v1);
        REQUIRE(c2 == c1);

        REQUIRE_THROWS(Chunk::deserialize(std::vector<uint8_t>(Chunk::header_size - 1, 1)));
        REQUIRE_NOTHROW(Chunk::deserialize(std::vector<uint8_t>(Chunk::header_size, 1)));
    }
}
