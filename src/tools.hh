#pragma once

#include "raw.hh"

namespace tools
{
void black_point_detection(const image::Gray16& im,
                           uint16_t& r_min,
                           uint16_t& g_min,
                           uint16_t& b_min);

void substract_min(const image::Gray16& im,
                   const uint16_t& r_min,
                   const uint16_t& g_min,
                   const uint16_t& b_min);

image::RGB16* debayering(const image::Gray16& im);
} // namespace tools