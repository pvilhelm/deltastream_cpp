#include "broadcast.h"

#include <stdexcept>

dstream::Broadcast::Broadcast() {
}


dstream::Broadcast::~Broadcast() {
}

std::shared_ptr<dstream::Node> dstream::Broadcast::getNode(Address address)
{
    auto it = this->map_of_nodes.find(address);
    if(it == this->map_of_nodes.end())
        throw(std::runtime_error("No such node"));
    else
        return it->second;
}

std::shared_ptr<dstream::Node> dstream::Broadcast::putNode(std::shared_ptr<Node> node)
{
    Address a = node->getAddress();
    auto it = this->map_of_nodes.find(a);
    if(it != this->map_of_nodes.end())
        throw(std::runtime_error("Node allready exists"));
    return this->map_of_nodes[a] = node;
    
}

std::shared_ptr<dstream::Node> dstream::Broadcast::hasNode(Address address)
{
    auto it = this->map_of_nodes.find(address);
    if(it == this->map_of_nodes.end())
        return 0;
    else
        return it->second;
}

std::shared_ptr<dstream::Node> dstream::Broadcast::deleteNode(Address address)
{
    auto it = this->map_of_nodes.find(address);
    if(it == this->map_of_nodes.end())
        throw(std::runtime_error("No such node"));
    auto node_ptr = it->second; /* Otherwise the node is deleted ... */
    this->map_of_nodes.erase(address);
    return node_ptr;
}
