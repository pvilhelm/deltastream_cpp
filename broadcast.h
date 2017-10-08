#pragma once

#include <map>
#include <memory>
#include <set>

#include "node.h"
#include "part.h"

namespace dstream{
    ///A representation of the whole broadcast.
    /**
     * 
     */
    class Broadcast {
    public:
        ///Broadcast identifier
        uint64_t id; 

        Broadcast();
        ~Broadcast();
        ///Gets pointer to node by #address.
        std::shared_ptr<Node> getNode(Address address);
        ///Puts a node into the broadcast. 
        /** 
         * \param node Pointer to node to add
         * \return Pointer to added node.
         * \throw runtime_error Throws if no such node exists.
         */
        std::shared_ptr<Node> putNode(std::shared_ptr<Node> node);
        ///Checks if there is a node added to the broadcast with the address.
        /** 
         * \param address Address to check for. 
         * \return Pointer to node if it exists or null pointer otherwise. 
         */
        std::shared_ptr<Node> hasNode(Address address);
        ///Deletes existing node in broadcast.
        /**
         * Deletes a node with Address address and throws if no
         * such node exists. 
         *
         * \param address Address of node to delete. 
         * \return Pointer to deleted node. 
         * \throw runtime_error Throws if no such node exists.
         */
        std::shared_ptr<Node> deleteNode(Address address);

    private: 
        std::map<Address,std::shared_ptr<Node>> map_of_nodes;
        std::set<uint32_t, Part> set_of_parts;
    };
}

