#include "chunk.h"

#include <stdexcept>

#include "Bytebuffer.h"

dstream::Chunk::Chunk(uint64_t broadcast_id, uint16_t chunk_nr, uint16_t total_chunks, chunk_type_E type, std::vector<uint8_t>& data) :
    broadcast_id(broadcast_id), chunk_nr(chunk_nr), total_chunks(total_chunks), type(type)
{
    this->data = data; 
}

dstream::Chunk::~Chunk()
{}

/*dstream::Chunk::Chunk()
{

};*/



dstream::Chunk dstream::Chunk::deserialize(const std::vector<uint8_t>& data)
{
    Bytebuffer b(std::make_shared<std::vector<uint8_t>>(data));

    
    if(data.size() < header_size)
        throw std::runtime_error("Invalid chunk length");

    uint64_t broadcast_id = b.get_uint64();
    uint16_t chunk_nr = b.get_uint16();
    uint16_t total_chunks = b.get_uint16();
    chunk_type_E type = (chunk_type_E)b.get_uint8();
    auto chunk_data = b.get_remainingBytes();

    Chunk c(broadcast_id, chunk_nr, total_chunks, type, std::move(chunk_data));

    return c; 
}

std::vector<uint8_t> dstream::Chunk::serialize()
{
    Bytebuffer b;
    b.put_uint64(this->broadcast_id);
    b.put_uint16(this->chunk_nr);
    b.put_uint16(this->total_chunks);
    b.put_uint8((uint8_t)this->type);
    b.put_bytes(this->data);
    return b.move_data();
}

bool dstream::operator==(const Chunk& lh, const Chunk& rh)
{
    if(rh.broadcast_id != lh.broadcast_id)
        return false;
    if(rh.chunk_nr != lh.chunk_nr)
        return false;
    if(rh.total_chunks != lh.total_chunks)
        return false;
    if(rh.type != lh.type)
        return false;
    if(rh.data != lh.data)
        return false;
    return true;
}