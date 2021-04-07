#include "tools.hh"

namespace tools
{
void black_point_detection(const image::Gray16& im,
                           uint16_t& r_min,
                           uint16_t& g_min,
                           uint16_t& b_min)
{
    r_min = 1023; // 2^10 - 1
    g_min = 1023;
    b_min = 1023;

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

void substract_min(const image::Gray16& im,
                   const uint16_t r_min,
                   const uint16_t g_min,
                   const uint16_t b_min)
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

static void interpolate_from_blue(const size_t y,
                                  const size_t x,
                                  const size_t,
                                  const size_t width,
                                  uint16_t* const in_data,
                                  uint16_t* const out_data)
{
    const size_t index = y * width + x;
    const size_t index_rgb = index * 3;

    uint8_t nb_pixel = 0;
    uint32_t value = 0;
    /* red */
    if (x > 0 && y > 0) // no overflow on the top left
    {
        value += in_data[index - 1 - width];
        nb_pixel++;
    }
    if (x < width - 1 && y > 0) // no overflow on the top right
    {
        value += in_data[index + 1 - width];
        nb_pixel++;
    }
    // no overflow on the bottom right
    value += in_data[index + 1 + width];
    nb_pixel++;

    if (x > 0) // no overflow on the bottom left (y < height - 1)
    {
        value += in_data[index - 1 + width];
        nb_pixel++;
    }

    out_data[index_rgb] = static_cast<uint16_t>(value / nb_pixel);

    /* green */
    nb_pixel = 0;
    value = 0;
    if (x > 0) // no overflow on the left
    {
        value += in_data[index - 1];
        nb_pixel++;
    }
    if (y > 0) // no overflow on the top
    {
        value += in_data[index - width];
        nb_pixel++;
    }
    // cannot overflow on the right
    value += in_data[index + 1];
    nb_pixel++;
    // cannot overflow on the bottom
    value += in_data[index + width];
    nb_pixel++;
    out_data[index_rgb + 1] = static_cast<uint16_t>(value / nb_pixel);

    /* blue */
    out_data[index_rgb + 2] = in_data[index];
}

static void interpolate_from_green(const size_t y,
                                   const size_t x,
                                   const size_t height,
                                   const size_t width,
                                   uint16_t* const in_data,
                                   uint16_t* const out_data)
{
    const size_t index = y * width + x;
    const size_t index_rgb = index * 3;

    uint8_t nb_pixel = 0;
    uint32_t value = 0;
    /* red or blue according to y (even or odd) */
    if (x > 0) // no overflow on the left
    {
        value += in_data[index - 1];
        nb_pixel++;
    }
    if (x < width - 1) // no overflow on the right
    {
        value += in_data[index + 1];
        nb_pixel++;
    }
    if (y % 2 == 0) // blue
        out_data[index_rgb + 2] = static_cast<uint16_t>(value / nb_pixel);
    else // red
        out_data[index_rgb] = static_cast<uint16_t>(value / nb_pixel);

    /* green */
    out_data[index_rgb + 1] = in_data[index];

    nb_pixel = 0;
    value = 0;
    /* red or blue according to y (even or odd) */
    if (y > 0) // no overflow on the top
    {
        value += in_data[index - width];
        nb_pixel++;
    }
    if (y < height - 1)
    {
        value += in_data[index + width];
        nb_pixel++;
    }
    if (y % 2 == 0) // red
        out_data[index_rgb] = static_cast<uint16_t>(value / nb_pixel);
    else // blue
        out_data[index_rgb + 2] = static_cast<uint16_t>(value / nb_pixel);
}
static void interpolate_from_red(const size_t y,
                                 const size_t x,
                                 const size_t height,
                                 const size_t width,
                                 uint16_t* const in_data,
                                 uint16_t* const out_data)
{
    const size_t index = y * width + x;
    const size_t index_rgb = index * 3;

    /* red */
    out_data[index_rgb] = in_data[index];

    /* green */
    uint8_t nb_pixel = 0;
    uint32_t value = 0;
    // cannot overflow on the left
    value += in_data[index - 1];
    nb_pixel++;
    if (x < width - 1) // no overflow on the right
    {
        value += in_data[index + 1];
        nb_pixel++;
    }
    // cannot overflow on the top
    value += in_data[index - width];
    nb_pixel++;
    if (y < height - 1) // no overflow on the bottom
    {
        value += in_data[index + width];
        nb_pixel++;
    }
    out_data[index_rgb + 1] = static_cast<uint16_t>(value / nb_pixel);

    /* blue */
    nb_pixel = 0;
    value = 0;
    // cannot overflow on the top left corner
    value += in_data[index - 1 - width];
    nb_pixel++;
    // No overflow on the top right corner (y > 0 always)
    if (x < width - 1)
    {
        value += in_data[index + 1 - width];
        nb_pixel++;
    }
    // No overflow on the bottom left corner (x > 0 always)
    if (y < height - 1)
    {
        value += in_data[index - 1 + width];
        nb_pixel++;
    }
    // No overflow on the bottom left corner (x > 0 always)
    if (x < width - 1 && y < height - 1)
    {
        value += in_data[index + 1 + width];
        nb_pixel++;
    }
    out_data[index_rgb + 2] = static_cast<uint16_t>(value / nb_pixel);
}

image::RGB16* debayering(const image::Gray16& in_im)
{
    const size_t height = in_im.height;
    const size_t width = in_im.width;
    assert(height % 2 == 0);
    assert(width % 2 == 0);

    image::RGB16* out_im = new image::RGB16(width, height);
    uint16_t* const out_data = out_im->data;
    uint16_t* const in_data = in_im.data;

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            if (x % 2 == 0 && y % 2 == 0) // Blue channel
                interpolate_from_blue(y, x, height, width, in_data, out_data);
            else if (x % 2 == 1 && y % 2 == 1) // Red channel
                interpolate_from_red(y, x, height, width, in_data, out_data);
            else // Green channel
                interpolate_from_green(y, x, height, width, in_data, out_data);
        }
    }
    return out_im;
}

} // namespace tools