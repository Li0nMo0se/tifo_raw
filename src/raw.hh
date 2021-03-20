#pragma once

#include <stddef.h>
#include <stdint.h>

namespace raw
{

template <typename T, unsigned int size>
struct Batch
{
    T data[size];
};

using InBatch = Batch<uint8_t, 5>;
using OutBatch = Batch<uint16_t, 4>;

struct Image
{
    Image(size_t size) { data = new uint16_t[size]; }
    ~Image() { delete[] data; }

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    uint16_t* data;
};

Image* get_image(const char* filename, const size_t width, const size_t height);

} // namespace raw