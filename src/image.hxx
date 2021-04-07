#include "image.hh"
#include <cassert>
#include <fstream>
#include <iostream>

namespace image
{

template <typename T, uint8_t depth>
void Image<T, depth>::save(const std::string& filename,
                           const uint8_t depth_bits) const
{
    std::ofstream of(filename.c_str(),
                     std::ios_base::out | std::ios_base::binary);

    if (of.fail())
    {
        std::cerr << "Cannot save the image in the file " << filename
                  << std::endl;
        return;
    }

    if constexpr (depth == 1) // grayscale
        of << "P5" << '\n';
    else if constexpr (depth == 3) // rgb
        of << "P6" << '\n';
    else
        assert(false); // Can't static assert here

    of << width << " " << height << '\n';
    constexpr unsigned int max_value = 255;
    of << max_value << '\n';

    // We only write 1 byte for each channel so we would need to right shift if
    // the data is larger than 1 byte
    const uint8_t shift = depth_bits - sizeof(uint8_t) * 8;
    for (size_t y = 0; y < height; y++) // height
    {
        for (size_t x = 0; x < width; x++) // width
        {
            const size_t index = y * width + x;
            for (uint8_t i = 0; i < depth; i++) // depth
            {
                const uint8_t value = data[index * depth + i] >> shift;
                assert(value <= max_value);
                of << value;
            }
        }
    }
    of.close();
}
} // namespace image