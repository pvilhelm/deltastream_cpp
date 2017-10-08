#include "node.h"

dstream::Node::Node() 
{ 
}


dstream::Node::~Node() 
{
}

dstream::Address dstream::Node::setAddress(IP ip, uint16_t port)
{
    this->address = dstream::Address(ip,port);
    return this->address;
}

dstream::Address dstream::Node::getAddress()
{
    return this->address;
}

dstream::Address::Address()
{}

dstream::Address::Address(uint32_t ip, uint16_t port) 
{
    this->port = port;
    this->ip.ip_v = IP4;
    this->ip.u_ip.ip4 = ip;
}

dstream::Address::Address(uint64_t *ip, uint16_t port)
{
    this->port = port;
    this->ip.ip_v = IP6;
    this->ip.u_ip.ip6[0] = ip[0];
    this->ip.u_ip.ip6[1] = ip[1];
}

dstream::Address::Address(IP ip, uint16_t port)
{
    this->port = port; 
    this->ip = ip;
}

dstream::Address::~Address()
{}


bool dstream::operator<(const Address& lh, const Address& rh)
{
    if(lh.ip.ip_v == IP4) {
        if(rh.ip.ip_v == IP4) {
            if(lh.ip.u_ip.ip4 < rh.ip.u_ip.ip4) {
                return true;
            } else if (lh.ip.u_ip.ip4 > rh.ip.u_ip.ip4) {
                return false;
            } else {
                return lh.port < rh.port;
            }
        } else { /* rh is ip6*/
            if(rh.ip.u_ip.ip6[2])
                return true;
            else if(lh.ip.u_ip.ip4 < rh.ip.u_ip.ip6[1])
                return true;
            else
                return lh.port < rh.port;
        }
    } else {
        if(rh.ip.ip_v == IP6)
            if(lh.ip.u_ip.ip6[1] < rh.ip.u_ip.ip6[1])
                return true;
            else
                if(lh.ip.u_ip.ip6[0] < rh.ip.u_ip.ip6[0])
                    return true;
                else if(lh.ip.u_ip.ip6[0] > rh.ip.u_ip.ip6[0])
                    return false; 
                else
                    return lh.port < rh.port;
        else
            if(lh.ip.u_ip.ip6[1])
                return false;
            else
                if(lh.ip.u_ip.ip6[0] < rh.ip.u_ip.ip4)
                    return true;
                else if((lh.ip.u_ip.ip6[0] > rh.ip.u_ip.ip4))
                    return false;
                else
                    return lh.port < rh.port;
    }
}

bool dstream::operator==(const Address& lh, const Address& rh)
{

    if(lh.ip.ip_v != rh.ip.ip_v)
        return false; 
    else if(lh.ip.ip_v == IP4) {
        if(lh.ip.u_ip.ip4 != rh.ip.u_ip.ip4)
           return false;
        else if(lh.port != rh.port)
           return false;
        return true;
    } else {
        if(lh.ip.u_ip.ip6[0] != rh.ip.u_ip.ip6[0])
            return false;
        else if(lh.ip.u_ip.ip6[1] != rh.ip.u_ip.ip6[1])
            return false;
        else if(lh.port != rh.port)
            return false;
        else
            return true;
    }
}

uint16_t dstream::Address::getPort()
{
    return this->port;
}

dstream::IP dstream::Address::getIP()
{
    return this->ip;
}

uint32_t dstream::Address::getIPv4()
{
    if(this->ip.ip_v == IP4)
        return this->ip.u_ip.ip4;
    else
        return 0; 
}

bool dstream::Address::isIPv4()
{
    return this->ip.ip_v == IP4;
}

bool dstream::operator>(const Address& lh, const Address& rh) { return rh < lh; }
bool dstream::operator!=(const Address& lh, const Address& rh) { return !(lh == rh); }
bool dstream::operator<=(const Address& lh, const Address& rh) { return !(lh > rh); }
bool dstream::operator>=(const Address& lh, const Address& rh) { return !(lh < rh); }
