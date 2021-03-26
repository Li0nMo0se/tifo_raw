#pragma once

#include "raw.hh"

namespace tools
{
void black_point_detection(const raw::Image& im,
                           uint16_t& r_min,
                           uint16_t& g_min,
                           uint16_t& b_min);
}