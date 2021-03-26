#include "image.hh"
#include <cassert>
#include <fstream>
#include <iostream>

namespace image
{
void Image::save(const std::string& filename) const
{
    std::ofstream of(filename.c_str(),
                     std::ios_base::out | std::ios_base::binary);

    if (of.fail())
    {
        std::cerr << "Cannot save the image in the file " << filename
                  << std::endl;
        return;
    }

    of << "P5" << std::endl;
    of << width << " " << height << std::endl;

    // 2^10 - 1 as it is encoded on 10 bits
    const uint16_t max_value = (1 << 10) - 1;
    of << max_value << std::endl;

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            assert(data[y * width + x] <= max_value);
            of << data[y * width + x];
        }
    }
    of.close();
}
} // namespace image