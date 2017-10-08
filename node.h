#pragma once

#include <cinttypes>

namespace dstream{
    enum ip_v_E
    {
        IP4,
        IP6
    };

    union ip_U
    {
        uint64_t ip6[2]; /* LE */
        uint32_t ip4; /* LE */
    };

    struct IP
    {
        ip_v_E ip_v; /* Ip version */
        ip_U u_ip;
         
        IP()
        {
            this->ip_v = IP4;
        }
        explicit IP(uint32_t ip)
        {
            this->ip_v = IP4;
            this->u_ip.ip4 = ip;
        }
        IP(uint64_t const * const ip)
        {
            this->ip_v = IP6;
            this->u_ip.ip6[0] = ip[0];
            this->u_ip.ip6[1] = ip[1];
        }

        bool operator==(const IP& src)
        {
            if(this->ip_v == IP4) {
                if(src.ip_v == IP6)
                    return false;
                else
                    return src.u_ip.ip4 == this->u_ip.ip4;
            } else {
                if(src.ip_v == IP4)
                    return false; 
                else      
                    return src.u_ip.ip6[0] == this->u_ip.ip6[0] &&
                        src.u_ip.ip6[1] == this->u_ip.ip6[1];
            }
        }
    };



    /// Ip and port class.
    /**
     * An address represents ip and port and hold information of 
     * ip protocol version.
     */
    class Address
    {
    public:
        ///Creates an empty andress.
        Address(); 
        ///Creates an ipv4 address.
        /**
         * \param ip Little endian ipv4.
         * \param port Remote port.
         */
        Address(uint32_t ip, uint16_t port);

        ///Creates an ipv6 address.
        /**
         * \param ip Little endian IPv6 uint64 array.
         * \param port remote port.
         */
        Address(uint64_t *ip, uint16_t port);
        ///Creates an ipv6 or ipv4 address.
        /**
         * \param ip IP object.
         * \param port Remote port. 
         */
        Address(IP ip, uint16_t port);
        ~Address();
        ///Compare addresses.
        /**
         * On ties the port number decides which address is lesser.
         */
        

        uint16_t getPort(); ///Gets the remote port.
        ///Gets ip.
        IP getIP(); 
        //Gets ip as IPv4.
        /** If the object is not IPv4 the functions returns 0. */
        uint32_t getIPv4();
        bool isIPv4();///True if the address is IPv4.

    //private:
        IP ip;
        uint16_t port; /* Port if relevant */
    };
    ///Overriden mostly for std::map usage.
    bool operator<(const Address& lh, const Address& rh);
    bool operator==(const Address& lh, const Address& rh);
    bool operator>(const Address& lh, const Address& rh);
    bool operator!=(const Address& lh, const Address& rh);
    bool operator<=(const Address& lh, const Address& rh);
    bool operator>=(const Address& lh, const Address& rh);

    /// A node in the broadcast.
    /**
     *  A node represents a participant in the broadcast. 
     *
     */
    class Node {
    public:
        /// Creates a empty node.
        Node();
        ~Node();
        /// Sets the IP and port of the node.
        /** 
         * \param ip The IP of the node. 
         * \param port Remote port. 
         */
        Address setAddress(IP ip, uint16_t port);
        /// Gets the ip and port of the node.
        Address getAddress();
    private:
        Address address;

    };
}

