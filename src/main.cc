#include "raw.hh"
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

    raw::Image* raw_image = raw::get_raw_image(argv[1], width, height);

    raw_image->save(std::string(argv[1]) + ".new");

    delete raw_image;

    return 0;
}
