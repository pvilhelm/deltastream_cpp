#include "Bytebuffer.h"

#include <stdexcept>

dstream::Bytebuffer::Bytebuffer()
{
    this->v_data = std::make_shared<std::vector<uint8_t>>();
    this->pos = 0;
    this->setBE();
}

dstream::Bytebuffer::Bytebuffer(std::shared_ptr<std::vector<uint8_t>> v_data)
{
    this->v_data = v_data;
    this->pos = 0;
    this->setBE();
}

dstream::Bytebuffer::~Bytebuffer()
{}

void dstream::Bytebuffer::setLE()
{
    this->E = LE;
}

void dstream::Bytebuffer::setBE()
{
    this->E = BE;
}

size_t dstream::Bytebuffer::rewind()
{
    return this->pos = 0;
}

size_t dstream::Bytebuffer::rewind(size_t n)
{
    if(n > this->pos)
        throw(std::runtime_error("Trying to rewind bytebuffer to before first element"));
    return this->pos -= n;
}

uint8_t dstream::Bytebuffer::get_uint8()
{
    if(this->pos + sizeof(uint8_t) > this->v_data->size())
        throw(std::runtime_error("Out of range"));

    return this->v_data->at(this->pos++);
    
    
}

uint16_t dstream::Bytebuffer::get_uint16()
{
    if(this->pos + sizeof(uint16_t) > this->v_data->size())
        throw(std::runtime_error("Out of range"));

    uint16_t ans = this->v_data->at(this->pos++);
    ans |= (uint16_t)this->v_data->at(this->pos++) << 8;
    if(this->E == LE) {
        return ans;
    } else {
        return change_endian(ans);
    }
}

uint32_t dstream::Bytebuffer::get_uint32()
{
    if(this->pos + sizeof(uint32_t) > this->v_data->size())
        throw(std::runtime_error("Out of range"));

    uint32_t ans = this->v_data->at(this->pos++);
    ans |= (uint32_t)this->v_data->at(this->pos++) << 8;
    ans |= (uint32_t)this->v_data->at(this->pos++) << 16;
    ans |= (uint32_t)this->v_data->at(this->pos++) << 24;
    if(this->E == LE) {
        return ans;
    } else {
        return change_endian(ans);
    }
}

uint64_t dstream::Bytebuffer::get_uint64()
{
    if(this->pos + sizeof(uint64_t) > this->v_data->size())
        throw(std::runtime_error("Out of range"));

    uint64_t ans = this->v_data->at(this->pos++);
    ans |= (uint64_t)this->v_data->at(this->pos++) << 8;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 16;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 24;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 32;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 40;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 48;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 56;
    
    if(this->E == LE) {
        return ans;
    } else {
        return change_endian(ans);
    }
}

int8_t dstream::Bytebuffer::get_int8()
{
    return this->get_uint8();
}

int16_t dstream::Bytebuffer::get_int16()
{
    return this->get_uint16();
}

int32_t dstream::Bytebuffer::get_int32()
{
    return this->get_uint32();
}

int64_t dstream::Bytebuffer::get_int64()
{
    return this->get_uint64();
}

float dstream::Bytebuffer::get_float()
{
    if(this->pos + sizeof(uint32_t) > this->v_data->size())
        throw(std::runtime_error("Out of range"));

    uint32_t ans = this->v_data->at(this->pos++);
    ans |= (uint32_t)this->v_data->at(this->pos++) << 8;
    ans |= (uint32_t)this->v_data->at(this->pos++) << 16;
    ans |= (uint32_t)this->v_data->at(this->pos++) << 24;

    if(this->E = LE) {
        float* ptr = (float*)&ans;
        return *ptr;
    } else {
        return fchange_endian(ans);
    }
}

double dstream::Bytebuffer::get_double()
{
    if(this->pos + sizeof(uint64_t) > this->v_data->size())
        throw(std::runtime_error("Out of range"));

    uint64_t ans = this->v_data->at(this->pos++);
    ans |= (uint64_t)this->v_data->at(this->pos++) << 8;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 16;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 24;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 32;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 40;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 48;
    ans |= (uint64_t)this->v_data->at(this->pos++) << 56;

    if(this->E = LE) {
        double* ptr = (double*)&ans;
        return *ptr;
    } else {
        return dchange_endian(ans);
    }
}

std::vector<uint8_t> dstream::Bytebuffer::get_bytes(size_t n)
{
    if(this->pos + n > this->v_data->size())
        throw(std::runtime_error("Out of range"));

    std::vector<uint8_t> v(n);
    for(auto& it : v) {
        it = this->get_uint8();
    }

    return v;
}

std::vector<uint8_t> dstream::Bytebuffer::get_remainingBytes()
{
    std::vector<uint8_t> v(v_data->begin() + pos, v_data->end());
    return v;
}

size_t dstream::Bytebuffer::setPos(size_t n)
{
    if(n > this->v_data->size())
        throw(std::runtime_error("Out of range"));
    return this->pos = n;
}

size_t dstream::Bytebuffer::skip(size_t n)
{
    if(this->pos + n > this->v_data->size())
        throw(std::runtime_error("Out of range"));
    return this->pos += n;
}

uint16_t dstream::change_endian(uint16_t i)
{
    uint16_t low = i & 0x00FFU;
    uint16_t high = i & 0xFF00U;
    return (low << 8) | (high >> 8);
}

uint32_t dstream::change_endian(uint32_t i)
{
    uint32_t out = (i & 0xFFU) << 24;
    out |= (i & 0xFF00U) << 8;
    out |= (i & 0xFF0000U) >> 8;
    return out |= (i & 0xFF000000U) >> 24;
}

uint64_t dstream::change_endian(uint64_t i)
{
    uint64_t out = (i & 0xFF) << 56;
    out |= (i & 0xFF00U) << 40;
    out |= (i & 0xFF0000U) << 24;
    out |= (i & 0xFF000000U) << 8;
    out |= (i & 0xFF00000000U) >> 8;
    out |= (i & 0xFF0000000000U) >> 24;
    out |= (i & 0xFF000000000000U) >> 40;
    return out |= (i & 0xFF00000000000000U) >> 56;
}

int16_t dstream::change_endian(int16_t i)
{
    return (int16_t)change_endian((uint16_t)i);
}

int32_t dstream::change_endian(int32_t i)
{
    return (int32_t)change_endian((uint32_t)i);
}

int64_t dstream::change_endian(int64_t i)
{
    return (int64_t)change_endian((uint64_t)i);
}

float dstream::fchange_endian(uint32_t f)
{
    uint32_t tmp = change_endian(f);
    float* ans = (float*)&tmp;
    return *ans;
}

double dstream::dchange_endian(uint64_t d)
{
    uint64_t tmp = change_endian(d);
    double* ans = (double*)&tmp;
    return *ans;
}

void dstream::Bytebuffer::insert_uint8(uint8_t i)
{
    v_data->insert(v_data->begin()+pos++, i);
}

void dstream::Bytebuffer::insert_uint16(uint16_t i)
{
    uint16_t tmp;
    uint8_t ii;

    if(E == BE)
        tmp = change_endian(i);
    else
        tmp = i;

    ii = tmp & 0xFFU;
    v_data->insert(v_data->begin() + pos++, ii); /* begin() iterator invalid after each insert*/
    ii = (tmp & 0xFF00U) >> 8U;
    v_data->insert(v_data->begin() + pos++, ii);
}

void dstream::Bytebuffer::insert_uint32(uint32_t i)
{
    uint32_t tmp;
    uint8_t ii;

    if(E == BE)
        tmp = change_endian(i);
    else
        tmp = i;

    ii = tmp & 0xFFU;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF00U) >> 8U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF0000U) >> 16U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF000000U) >> 24U;
    v_data->insert(v_data->begin() + pos++, ii);
}

void dstream::Bytebuffer::insert_uint64(uint64_t i)
{
    uint64_t tmp;
    uint8_t ii;

    if(E == BE)
        tmp = change_endian(i);
    else
        tmp = i;

    ii = tmp & 0xFFU;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF00U) >> 8U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF0000U) >> 16U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF000000U) >> 24U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF00000000U) >> 32U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF0000000000U) >> 40U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF000000000000U) >> 48U;
    v_data->insert(v_data->begin() + pos++, ii);
    ii = (tmp & 0xFF00000000000000U) >> 56U;
    v_data->insert(v_data->begin() + pos++, ii);
}

void dstream::Bytebuffer::insert_int8(int8_t i)
{
    insert_uint8((uint8_t)i);
}

void dstream::Bytebuffer::insert_int16(int16_t i)
{
    insert_uint16((uint16_t)i);
}

void dstream::Bytebuffer::insert_int32(int32_t i)
{
    insert_uint32((uint32_t)i);
}

void dstream::Bytebuffer::insert_int64(int64_t i)
{
    insert_uint64((uint64_t)i);
}

void dstream::Bytebuffer::insert_float(float i)
{
    insert_uint32(*(uint32_t*)&i);
}

void dstream::Bytebuffer::insert_double(double i)
{
    insert_uint64(*(uint64_t*)&i);
}

void dstream::Bytebuffer::insert_bytes(std::vector<uint8_t> v)
{
    for(uint8_t i : v)
        v_data->insert(v_data->begin()+pos++,i);
}

void dstream::Bytebuffer::put_uint8(uint8_t i)
{
    pos = v_data->size();
    insert_uint8(i);
}

void dstream::Bytebuffer::put_uint16(uint16_t i)
{
    pos = v_data->size();
    insert_uint16(i);
}

void dstream::Bytebuffer::put_uint32(uint32_t i)
{
    pos = v_data->size();
    insert_uint32(i);
}

void dstream::Bytebuffer::put_uint64(uint64_t i)
{
    pos = v_data->size();
    insert_uint64(i);
}

void dstream::Bytebuffer::put_int8(int8_t i)
{
    pos = v_data->size();
    insert_int8(i);
}

void dstream::Bytebuffer::put_int16(int16_t i)
{
    pos = v_data->size();
    insert_int16(i);
}

void dstream::Bytebuffer::put_int32(int32_t i)
{
    pos = v_data->size();
    insert_int32(i);
}

void dstream::Bytebuffer::put_int64(int64_t i)
{
    pos = v_data->size();
    insert_int64(i);
}

void dstream::Bytebuffer::put_float(float i)
{
    pos = v_data->size();
    insert_uint32(*(uint32_t*)&i);
}

void dstream::Bytebuffer::put_double(double i)
{
    pos = v_data->size();
    insert_int64(*(uint64_t*)&i);
}

void dstream::Bytebuffer::put_bytes(std::vector<uint8_t> v)
{
    pos = v_data->size();
    insert_bytes(v);
}

dstream::ByteBufferIterator::ByteBufferIterator(uint8_t *ptr)
{
    this->ptr = ptr;
}

dstream::ByteBufferIterator::reference dstream::ByteBufferIterator::operator*() const
{
    return *ptr;
}


dstream::ByteBufferIterator &dstream::ByteBufferIterator::operator++()
{
    ++ptr;
    return *this;
}

dstream::ByteBufferIterator dstream::ByteBufferIterator::operator++(int)
{
    auto tmp = *this;
    ++ptr;
    return tmp;
}

dstream::ByteBufferIterator & dstream::ByteBufferIterator::operator--()
{
    return --ptr, *this;
}

dstream::ByteBufferIterator dstream::ByteBufferIterator::operator--(int)
{
    auto tmp = *this; 
    --ptr;
    return *this;
}

bool dstream::ByteBufferIterator::operator==(const ByteBufferIterator & rhs)
{
    return this->ptr == rhs.ptr;
}

bool dstream::ByteBufferIterator::operator!=(const ByteBufferIterator & rhs)
{
    return !(*this == rhs);
}

