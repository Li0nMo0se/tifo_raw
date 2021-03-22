#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string>

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
    Image(const size_t _width, const size_t _height)
        : width(_width)
        , height(_height)
    {
        data = new uint16_t[width * height];
    }
    ~Image() { delete[] data; }

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    // Save to PGM (grayscale PPM)
    void save(const std::string& filename) const;

    const size_t width;
    const size_t height;
    uint16_t* data;
};

Image* get_raw_image(const std::string& filename,
                     const size_t width,
                     const size_t height);

} // namespace raw