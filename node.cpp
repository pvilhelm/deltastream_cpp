#include "node.h"

dstream::node::node(uint32_t ip, uint16_t port) 
{
    this->address = dstream::address(ip, port);
}


dstream::node::~node() 
{
}

dstream::address dstream::node::setAddress(uint32_t ip, uint16_t port)
{
    this->address = dstream::address(ip,port);
    return this->address;
}

dstream::address dstream::node::getAddress()
{
    return this->address;
}

dstream::address::address()
{}

dstream::address::address(uint32_t ip, uint16_t port) 
{
    this->port = port;
    this->ip_v = IP4;
    this->ip.ip4 = ip;
}

dstream::address::address(ip6 ip, uint16_t port) 
{
    this->port = port; 
    this->ip_v = IP6;
    this->ip.ip6[0] = ip.arr[0];
    this->ip.ip6[1] = ip.arr[1];
}

dstream::address::~address()
{}


bool dstream::address::operator<(const address & rh) 
{
    if (this->ip_v == IP4) {
        if (rh.ip_v == IP4) {
            if (this->ip.ip4 < rh.ip.ip4) {
                return true;
            } else if (this->ip.ip4 > rh.ip.ip4) {
                return false;
            } else {
                return this->port < rh.port;
            }
        } else { /* rh is ip6*/
            if (rh.ip.ip6[2])
                return true;
            else
                return this->ip.ip4 < rh.ip.ip6[1];
        }
    } else {
        if (rh.ip_v == IP6)
            if (this->ip.ip6[1] < rh.ip.ip6[1])
                return false;
            else
                return this->ip.ip6[0] < rh.ip.ip6[0];
        else
            if (this->ip.ip6[1])
                return true;
            else
                return this->ip.ip6[0] < rh.ip.ip4;
    }
}

uint16_t dstream::address::getPort()
{
    return this->port;
}

dstream::ip6 dstream::address::getIP6()
{
    if (this->ip_v == IP6)
        return dstream::ip6(this->ip.ip6);
    else
        return dstream::ip6();
}

uint32_t dstream::address::getIP4()
{
    if (this->ip_v == IP4)
        return this->ip.ip4;
    else
        return 0;
}

bool dstream::address::isIP4()
{
    return this->ip_v == IP4;
}


