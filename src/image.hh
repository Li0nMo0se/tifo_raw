#pragma once

#include <stdint.h>
#include <string>
namespace image
{

template <typename T>
concept Integral = std::is_integral<T>::value;

template <Integral T, uint8_t depth>
struct Image
{
    Image(const size_t _width, const size_t _height)
        : width(_width)
        , height(_height)
    {
        data = new T[width * height * depth];
    }
    ~Image() { delete[] data; }

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    // Save to PGM (grayscale PPM)
    void save(const std::string& filename, const uint8_t depth_bits) const;

    const size_t width;
    const size_t height;
    T* data;
};

using Gray16 = Image<uint16_t, 1>;
using RGB16 = Image<uint16_t, 3>;
} // namespace image

#include "image.hxx"