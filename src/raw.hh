#pragma once

#include "image.hh"
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

image::Gray16* get_raw_image(const std::string& filename,
                             const size_t width,
                             const size_t height);

} // namespace raw