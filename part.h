#pragma once

#include <memory>
#include <set>

#include "chunk.h"

namespace dstream{
    ///Representation of a "part" of the transmitted stream.
    /** The parts together in order recreates the transmitted stream from the 
     * source node. 
     */
    class Part
    {
 
        public: 
            Part();
            ~Part();

        private: 
            bool is_complete;
            std::set<Chunk> set_of_chunks;
    };
}