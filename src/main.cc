#include "raw.hh"
#include "tools.hh"
#include <iostream>
#include <sstream>

int main(int argc, const char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << "filename.raw width height\n";
        return 1;
    }

    size_t width;
    std::stringstream ss_width(argv[2]);
    ss_width >> width;
    size_t height;
    std::stringstream ss_height(argv[3]);
    ss_height >> height;

    constexpr uint8_t depth_bits = 10;
    image::Gray16* raw_image = raw::get_raw_image(argv[1], width, height);
    raw_image->save(std::string(argv[1]) + ".pgm", depth_bits);

    // Black point detection
    uint16_t r_min;
    uint16_t g_min;
    uint16_t b_min;
    tools::black_point_detection(*raw_image, r_min, g_min, b_min);
    tools::substract_min(*raw_image, r_min, g_min, b_min);

    image::RGB16* rgb_image = tools::debayering(*raw_image);

    rgb_image->save(std::string(argv[1]) + ".ppm", depth_bits);

    delete raw_image;
    delete rgb_image;

    return 0;
}
