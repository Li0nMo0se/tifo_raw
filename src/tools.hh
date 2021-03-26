#pragma once

#include "raw.hh"

namespace tools
{
void black_point_detection(const image::Image& im,
                           uint16_t& r_min,
                           uint16_t& g_min,
                           uint16_t& b_min);

void substract_min(const image::Image& im,
                   const uint16_t& r_min,
                   const uint16_t& g_min,
                   const uint16_t& b_min);

// debayering(const raw::Image& im) {}
} // namespace tools