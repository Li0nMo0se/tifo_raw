#include "tools.hh"

namespace tools
{
void black_point_detection(const image::Image& im,
                           uint16_t& r_min,
                           uint16_t& g_min,
                           uint16_t& b_min)
{
    r_min = image::Image::max_value;
    g_min = image::Image::max_value;
    b_min = image::Image::max_value;

    for (size_t y = 0; y < im.height; y++)
    {
        for (size_t x = 0; x < im.width; x++)
        {
            size_t index = y * im.width + x;
            if (x % 2 == 0 && y % 2 == 0) // Blue channel
                b_min = std::min(im.data[index], b_min);
            else if (x % 2 == 1 && y % 2 == 1) // Red channel
                r_min = std::min(im.data[index], r_min);
            else // Green channel
                g_min = std::min(im.data[index], g_min);
        }
    }
}

void substract_min(const image::Image& im,
                   const uint16_t& r_min,
                   const uint16_t& g_min,
                   const uint16_t& b_min)
{
    for (size_t y = 0; y < im.height; y++)
    {
        for (size_t x = 0; x < im.width; x++)
        {
            size_t index = y * im.width + x;
            if (x % 2 == 0 && y % 2 == 0) // Blue channel
                im.data[index] -= b_min;
            else if (x % 2 == 1 && y % 2 == 1) // Red channel
                im.data[index] -= r_min;
            else // Green channel
                im.data[index] -= g_min;
        }
    }
}

} // namespace tools