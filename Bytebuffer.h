#pragma once 

#include <memory>
#include <vector>
#include <cstdint>

namespace dstream {
 
    uint16_t change_endian(uint16_t i);
    uint32_t change_endian(uint32_t i);
    uint64_t change_endian(uint64_t i);
    int16_t change_endian(int16_t i);
    int32_t change_endian(int32_t i);
    int64_t change_endian(int64_t i);
    float fchange_endian(uint32_t f);
    double dchange_endian(uint64_t d);

    enum endianess_E
    {
        BE,
        LE
    };

    class ByteBufferIterator
    {
    public:
        // Iterator traits, previously from std::iterator.
        using value_type = uint8_t;
        using difference_type = std::ptrdiff_t;
        using pointer = uint8_t*;
        using reference = uint8_t&;
        using iterator_category = std::bidirectional_iterator_tag;

        // Default constructible.
        ByteBufferIterator() = default;
        explicit ByteBufferIterator(uint8_t *ptr);

        // Dereferencable.
        reference operator*() const;

        // Pre- and post-incrementable.
        ByteBufferIterator& operator++();
        ByteBufferIterator operator++(int);

        // Pre- and post-decrementable.
        ByteBufferIterator& operator--();
        ByteBufferIterator operator--(int);

        // Equality / inequality.
        bool operator==(const ByteBufferIterator& rhs);
        bool operator!=(const ByteBufferIterator& rhs);
    private:
        uint8_t *ptr;


    };


    ///Bytebuffer for convenient network packet parsing.
    /** The buffer defaults to big-endian mode where it inputs/outputs data with it's endianess
     * changed. In little-endian mode the buffer inputs/outputs the data unaltered. 
     *
     * The buffer tracks the "position", i.e. offset in memory on the buffer's backing array, on which work is done. 
     *
     * An "element" is a type of varying size. "End" is the position after the last byte, of an element or
     * the backing buffer array.
     *
     * "Insert" inserts an element before the buffer position and after the preceding position and then 
     * moves the position forward to
     * the end of the element. I.e. the element get jammed in between other elements (or start/end w/e). 
     *
     * "Put" inserts an element at the end of the buffer and then moves the position to the end
     * of the buffer. 
     *
     * "Get" gets an element at the position and moves the position forward to the end of the element. 
     */
    class Bytebuffer
    {
    public:

        using iterator = ByteBufferIterator;
    
        Bytebuffer();
        Bytebuffer(std::shared_ptr<std::vector<uint8_t>> v_data);
        ~Bytebuffer();
        ///Set the Bytebuffer to Little-endian (LE) byte order.
        /** This can be done at any time and the next reads will be LE. */
        void setLE();
        ///Set the Bytebuffer to Big-endian (BE) byte order.
        /** This can be done at any time and the next reads will be BE. */
        void setBE();

        ///Reset the buffer position to the start. 
        /** \returns Position after rewind. */
        size_t rewind();
        ///Reverse the buffer position by n steps. 
        /** \param n Steps to reverse.
         * \throws runtime_error Throws if n would go before the start position. 
         * \returns Position after rewind. 
         */
        size_t rewind(size_t n);
        ///Get uint8_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        uint8_t get_uint8();
        ///Get uint16_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        uint16_t get_uint16();
        ///Get uint32_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        uint32_t get_uint32();
        ///Get uint64_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        uint64_t get_uint64();
        ///Get int8_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        int8_t get_int8();
        ///Get int16_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        int16_t get_int16();
        ///Get int32_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        int32_t get_int32();
        ///Get int64_t and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        int64_t get_int64();
        ///Get float (single) and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        float get_float();
        ///Get double and advance buffer position.
        /** \throws runtime_error Throws if the read would go beyond the data. */
        double get_double();

        ///Get bytes as a vector.
        /** \param n Amount of bytes to get 
         *  \throws runtime_error Throws if the read would go beyond the data.
         */
        std::vector<uint8_t> get_bytes(size_t n);
        ///Get remaining bytes.
        /** \return Vector with bytes from position to the last byte. */
        std::vector<uint8_t> get_remainingBytes();

        ///Get current buffer position.
        /** \returns The position zero indexed from start.
         */
        size_t getPos(){return pos;}
        ///Set the buffer position.
        /** \param n The target position, zero indexed from start.
         * \throws runtime_error Throws if the new position go beyond the end. 
         * \return The new position.
         */
         size_t setPos(size_t n);
         ///Skips n positions forwards.
         /** \param n Number of positions to skip.
          *  \throws runtime_error Throws if the new position go beyond the end. 
          *  \return The new position.
          */
         size_t skip(size_t n);
         ///Skips positions to end of buffer.
         /**
          * \return The position after the last data. 
          */
         size_t skip(){ return pos = v_data->size(); }

         ///Checks if the buffer position is at the end.
         bool isEnd(){ return pos == v_data->size(); }
         ///Get size in bytes of buffer.
         size_t getSize(){ return v_data->size(); }
         ///Get size left to end of buffer.
         size_t getSizeLeft(){ return v_data->size() - pos; }

         ///Insert a element before position and increase position.
         void insert_uint8(uint8_t i);
         ///Insert a element before position and increase position.
         void insert_uint16(uint16_t i);
         ///Insert a element before position and increase position.
         void insert_uint32(uint32_t i);
         ///Insert a element before position and increase position.
         void insert_uint64(uint64_t i);
         ///Insert a element before position and increase position.
         void insert_int8(int8_t i);
         ///Insert a element before position and increase position.
         void insert_int16(int16_t i);
         ///Insert a element before position and increase position.
         void insert_int32(int32_t i);
         ///Insert a element before position and increase position.
         void insert_int64(int64_t i);
         ///Insert a element before position and increase position.
         void insert_float(float i);
         ///Insert a element before position and increase position.
         void insert_double(double i);
         ///Insert a element before position and increase position.
         void insert_bytes(std::vector<uint8_t> v);

         ///Put a element in the end of the buffer and move position to new end. 
         void put_uint8(uint8_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_uint16(uint16_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_uint32(uint32_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_uint64(uint64_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_int8(int8_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_int16(int16_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_int32(int32_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_int64(int64_t i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_float(float i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_double(double i);
         ///Put a element in the end of the buffer and move position to new end.
         void put_bytes(std::vector<uint8_t> v);

         ///Get buffer byte by index.
         /** This function doesn't check the parameter and doesn't affect the position. 
          * Accessing elements beyond the last causes undefined behavior. 
          * \param i Index to get.
          * \return Reference to the byte.
          */
         uint8_t& operator[](size_t i)
         {
             return (*v_data)[i];
         }

         ///Retrieves the backing data by moving and invalidates it.
         /** Adding or retrieving data from the buffer after a move causes
          * undefined behavior. */
         std::vector<uint8_t> move_data(){ return std::move(*v_data); }
         ///Retrieves the backing data by copying it. 
         std::vector<uint8_t> copy_data(){ return *v_data; }


         /// Get bidirectional iterator to end.
         /** Mainly to allow for for range iterations. */
         iterator end()
         {
             return ByteBufferIterator(1 + &(*v_data)[v_data->size()-1]);
         }
         /// Get bidirectional iterator to beginning. 
         /** Mainly to allow for for range iterations. */
         iterator begin()
         {
             return ByteBufferIterator(&(*v_data)[0]);
         }

    private:

        size_t pos; /* Position of head */
        endianess_E E;

        std::shared_ptr<std::vector<uint8_t>> v_data;

    };
}

