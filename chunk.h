#pragma once

#include <cstdint>
#include <vector>

namespace dstream{
    
    enum chunk_type_E : uint8_t
    {
        UDP_DATA,
    };

    

    ///A chunk is a smaller "part" of a Part. 
    /** To transmitt parts they are divided into chunks and sent. 
     * Each chunk is supposed to fit in one transmission unit (e.g. a UDP package).
     */
    class Chunk
    {
    public:
        uint64_t broadcast_id; ///The broadcast the chunk is part of. 
        uint16_t chunk_nr;  ///The sequencial number of this chunk in the part.
        uint16_t total_chunks;///The number of the last chunk in the part. 
        chunk_type_E type;///The type of this chunk.
        std::vector<uint8_t> data;///The data field of the chunk.

        //Chunk();
        Chunk(uint64_t broadcast_id, uint16_t chunk_nr, uint16_t total_chunks, chunk_type_E type, std::vector<uint8_t>& data);
        ~Chunk();

        static Chunk deserialize(const std::vector<uint8_t>& data);
        std::vector<uint8_t> serialize();

        static const size_t header_size = sizeof(uint64_t) + sizeof(uint16_t) * 2 + sizeof(uint8_t);
    private: 
        
    };

    bool operator==(const Chunk& lh, const Chunk& rh);
}
