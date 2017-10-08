#include "catch.hpp"

#include <cstdint>

#include "Bytebuffer.h"

using namespace dstream;

TEST_CASE("bytebyffer")
{
    SECTION("change_endian")
    {
        uint16_t ui16 = 0x2211;
        uint32_t ui32 = 0x44332211;
        uint64_t ui64 = 0x8877665544332211;
        int16_t i16 = 0x2211;
        int32_t i32 = 0x44332211;
        int64_t i64 = 0x8877665544332211;

        REQUIRE(change_endian(ui16) == 0x1122);
        REQUIRE(change_endian(ui32) == 0x11223344);
        REQUIRE(change_endian(ui64) == 0x1122334455667788);
        REQUIRE(change_endian(i16) == 0x1122);
        REQUIRE(change_endian(i32) == 0x11223344);
        REQUIRE(change_endian(i64) == 0x1122334455667788);

        /* "Magic" numbers are just 0x1122334455667788 or 0x11223344 and reversed in d/f */
        float fnzero = -3.3360025E-13f;
        uint32_t* tmp = (uint32_t*)&fnzero;
        REQUIRE(fchange_endian(*tmp) == -1.84407149E18f);
        double dnzero = 3.841412024471730659E-226;
        uint64_t* tmp2 = (uint64_t*)&dnzero;
        REQUIRE(dchange_endian(*tmp2) == -7.0868766365730135833E-268);

    }

    SECTION("getX() rewind() skip() isEnd()")
    {
        {
            std::vector<uint8_t> v = {0x01, 0x01, 0x02, 0x01, 0x02, 0x03, 0x04,  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
            auto ptr = std::make_shared<std::vector<uint8_t>>(v);
            Bytebuffer b(ptr);
            
            b.setLE(); /* Begin with trying LE */


            auto v_back = b.get_remainingBytes();
            REQUIRE(v_back == v);
            b.rewind();

            /* Chech that the buffer is parsed properly */
            REQUIRE(b.get_uint8() == 0x01);
            REQUIRE(b.get_uint16() == 0x0201);
            REQUIRE(b.get_uint32() == 0x04030201);
            REQUIRE(b.get_uint64() == 0x0807060504030201);
            REQUIRE_THROWS(b.get_uint8());
            REQUIRE(b.getPos() == ptr->size()); /* Should be one beyond last */
            REQUIRE(b.rewind() == 0); /* Returns pos 0 */
            REQUIRE(b.get_int8() == 0x01);
            REQUIRE(b.get_int16() == 0x0201);
            REQUIRE(b.get_int32() == 0x04030201);
            REQUIRE(b.get_int64() == 0x0807060504030201);
        
            b.rewind();
            b.setBE(); /* Try BE */
            REQUIRE(b.get_uint8() == 1);
            REQUIRE(b.get_uint16() == 0x0102);
            REQUIRE(b.get_uint32() == 0x01020304);
            REQUIRE(b.get_uint64() == 0x0102030405060708);
            REQUIRE_THROWS(b.get_uint8());
            REQUIRE(b.isEnd());
            b.rewind();
            REQUIRE(b.get_int8() == 1);
            REQUIRE(b.get_int16() == 0x0102);
            REQUIRE(b.get_int32() == 0x01020304);
            REQUIRE(b.get_int64() == 0x0102030405060708);

            /* Let see so it throws properly when going over the end */
            b.rewind(1);
            REQUIRE_NOTHROW(b.get_uint8());
            b.rewind(1);
            size_t pos_old = b.getPos();
            REQUIRE_THROWS(b.get_uint16());
            REQUIRE(pos_old == b.getPos()); /* Check so pos doesnt change ... */
            b.rewind(2);
            REQUIRE_THROWS(b.get_uint32());
            b.rewind(3);
            REQUIRE_THROWS(b.get_uint64());

            /* Check get n bytes */
            {
                b.rewind();
                auto v2 = b.get_bytes(4);
                for(int i = 0; i < 4; i++)
                    REQUIRE(v2[i] == v[i]);
                REQUIRE_THROWS(b.get_bytes(1<<10));
            }

            { /* Check skip */
                b.rewind();
                b.skip(12);
                REQUIRE(b.get_uint8() == 6);
                REQUIRE_THROWS(b.skip(1<<10));
            }
        }
        { /* floats */
            auto ptr = std::make_shared<std::vector<uint8_t>>();

            Bytebuffer b(ptr);
            uint32_t i1 = 0x44332211;
            {
                float f1 = *(float*)&i1;
                uint8_t* arr = (uint8_t*)&i1;
                for(int i = 0; i < 4; i++)
                    ptr->push_back(arr[i]);
            
                
                b.setLE();
                REQUIRE(b.get_float() == f1);
            }
            {
                double f1 = *(double*)&i1;
                uint8_t* arr = (uint8_t*)&i1;
                for(int i = 0; i < 8; i++)
                    ptr->push_back(arr[i]);

                REQUIRE(b.get_double() == f1);
            }
        }
    }

    SECTION("insert_x() operator[]")
    {
        Bytebuffer b;

        b.setLE();
        REQUIRE_THROWS(b.rewind(1));
        b.insert_uint8(0x11U);
        b.insert_uint16(0x2222U);
        b.insert_uint32(0x33333333U);
        b.insert_uint64(0x4444444444444444U);
        b.insert_int8(0x11);
        b.insert_int16(0x2222);
        b.insert_int32(0x33333333);
        b.insert_int64(0x4444444444444444);
        b.insert_float(1.f);
        b.insert_double(2.);
        b.rewind();
        REQUIRE(b.get_uint8() == 0x11U);
        REQUIRE(b.get_uint16() == 0x2222U);
        REQUIRE(b.get_uint32() == 0x33333333U);
        REQUIRE(b.get_uint64() == 0x4444444444444444U);
        REQUIRE(b.get_int8() == 0x11);
        REQUIRE(b.get_int16() == 0x2222);
        REQUIRE(b.get_int32() == 0x33333333);
        REQUIRE(b.get_int64() == 0x4444444444444444);
        b.rewind(sizeof(uint64_t));
        size_t old = b.getSize();
        b.insert_uint32(0xffffffffu);
        REQUIRE(old + sizeof(uint32_t) == b.getSize());
        b.rewind(sizeof(uint32_t));
        REQUIRE(b.get_uint32() == 0xffffffffu);
        REQUIRE(b.get_int64() == 0x4444444444444444);
        REQUIRE(b.get_float() == 1.f);
        REQUIRE(b.get_double() == 2.);
        REQUIRE(b.isEnd());

        REQUIRE(b[0] == 0x11u);
        REQUIRE(b[3] == 0x33u);
    }
}