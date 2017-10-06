#pragma once

#include <cinttypes>

namespace dstream{
    struct ip6
    {
        uint64_t arr[2];
        ip6(uint64_t arr_ip[2])
        {
            this->arr[0] = arr_ip[0];
            this->arr[1] = arr_ip[1];
        }
        ip6()
        {
            this->arr[0] = 0;
            this->arr[1] = 0;
        }

        bool operator==(const ip6& src)
        {
            return src.arr[0] == this->arr[0] && src.arr[1] == this->arr[1];
        }
    };

    class address
    {
    public:
        address();
        address(uint32_t ip, uint16_t port);
        address(ip6 ip, uint16_t port);
        ~address();
        bool operator<(const address &rh);

        uint16_t getPort();
        ip6 getIP6();
        uint32_t getIP4();
        bool isIP4();

    private:
        enum ip_v_E
        {
            IP4,
            IP6
        } ip_v; /* Ip version */
        union u_ip
        {
            uint64_t ip6[2]; /* LE */
            uint32_t ip4; /* LE */
        } ip;
        uint16_t port; /* Port if relevant */
    };


    /** \brief A node in the broadcast.
     *
     *  A node represents a participant in the broadcast. 
     *
     */
    class node {
    public:
        node(uint32_t ip, uint16_t port);
        ~node();
        address setAddress(uint32_t ip, uint16_t port);
        address getAddress();
    private:
        address address;

    };
}

