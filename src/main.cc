#include "raw.hh"
#include "tools.hh"
#include <iostream>
#include <sstream>

int main(int argc, const char* argv[])
{
    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0]
                  << " filename.raw width height gain_r grain_b\n"
                  << "notice: metada can be found in data/*.meta\n";
        return 1;
    }

    // Get value
    size_t width;
    std::stringstream ss_width(argv[2]);
    ss_width >> width;
    size_t height;
    std::stringstream ss_height(argv[3]);
    ss_height >> height;
    float gain_r;
    std::stringstream ss_gain_r(argv[4]);
    ss_gain_r >> gain_r;
    float gain_b;
    std::stringstream ss_gain_b(argv[5]);
    ss_gain_b >> gain_b;

    constexpr uint8_t depth_bits = 10;
    image::Gray16* raw_image = raw::get_raw_image(argv[1], width, height);
    std::string saved_raw_image_path = std::string(argv[1]) + ".pgm";
    std::cout << "Saving raw image as pgm (" << saved_raw_image_path << ")\n";
    raw_image->save(saved_raw_image_path, depth_bits);

    // Black point detection
    uint16_t r_min;
    uint16_t g_min;
    uint16_t b_min;
    tools::black_point_detection(*raw_image, r_min, g_min, b_min);
    tools::substract_min(*raw_image, r_min, g_min, b_min);

    image::RGB16* rgb_image = tools::debayering(*raw_image);

    std::string saved_rgb_image_path = std::string(argv[1]) + ".ppm";
    std::cout << "Saving RGB image as ppm (" << saved_rgb_image_path << ")\n";
    rgb_image->save(saved_rgb_image_path, depth_bits);

    tools::white_balance(*rgb_image, gain_r, gain_b);

    std::string saved_white_balance_image_path = std::string(argv[1]) + ".whitebalance.ppm";
    std::cout << "Saving white balance RGB image as ppm ("
              << saved_white_balance_image_path << ")\n";
    rgb_image->save(saved_white_balance_image_path, depth_bits);

    delete raw_image;
    delete rgb_image;

    return 0;
}
