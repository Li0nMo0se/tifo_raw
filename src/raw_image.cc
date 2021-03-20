#include "raw_image.hh"
#include <cassert>
#include <cstdio>
#include <iostream>

namespace raw
{
static OutBatch read_batch(const InBatch& in)
{
    // Make the interpretation of the input batch of bytes (size: 5)
    // Return the output batch according to the input (size: 4)

    // Read every byte
    const uint8_t byte_1 = in.data[0];
    const uint8_t byte_2 = in.data[1];
    const uint8_t byte_3 = in.data[2];
    const uint8_t byte_4 = in.data[3];
    const uint8_t byte_5 = in.data[4];

    // Create mask
    static constexpr uint8_t mask_pixel_1 = (1 << 7) | (1 << 6);
    static constexpr uint8_t mask_pixel_2 = (1 << 5) | (1 << 4);
    static constexpr uint8_t mask_pixel_3 = (1 << 3) | (1 << 2);
    static constexpr uint8_t mask_pixel_4 = (1 << 1) | (1 << 0);

    // Get the value of the ouput pixel
    OutBatch out;
    out.data[0] =
        static_cast<uint16_t>(byte_1 << 2) | ((byte_5 & mask_pixel_1) >> 6);
    out.data[1] =
        static_cast<uint16_t>(byte_2 << 2) | ((byte_5 & mask_pixel_2) >> 4);
    out.data[2] =
        static_cast<uint16_t>(byte_3 << 2) | ((byte_5 & mask_pixel_3) >> 2);
    out.data[3] =
        static_cast<uint16_t>(byte_4 << 2) | ((byte_5 & mask_pixel_4) >> 0);

    return out;
}

Image*
get_raw_image(const char* filename, const size_t width, const size_t height)
{
    // Open file
    FILE* f = std::fopen(filename, "r");
    if (!f)
        std::cerr << "ERROR: cannot open " << filename << " for writing!\n";

    size_t in_size = width * height;
    // Store input raw after decoding the input
    Image* in_image = new Image(in_size);

    // Decode input
    size_t curr_index = 0;
    InBatch curr_in_batch;
    // Read 5 bytes to 5 bytes
    size_t bytes_read = std::fread(curr_in_batch.data, 1, sizeof(InBatch), f);
    while (bytes_read > 0)
    {
        assert(bytes_read == sizeof(InBatch));
        // Decode the 5 bytes into 4 bytes
        OutBatch curr_out_batch = read_batch(curr_in_batch);
        // Store it in the raw
        for (uint8_t i = 0;
             i < sizeof(OutBatch) / sizeof(curr_out_batch.data[0]);
             i++)
        {
            assert(curr_index < in_size);
            in_image->data[curr_index++] = curr_out_batch.data[i];
        }
        bytes_read = std::fread(curr_in_batch.data, 1, sizeof(InBatch), f);
    }

    assert(curr_index == in_size); // Every pixel have been treated

    return in_image;
}

} // namespace raw