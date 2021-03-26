#pragma once

#include <stdint.h>
#include <string>

namespace image
{
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

    static constexpr uint16_t max_value = 1023; // 2^10
};
} // namespace image