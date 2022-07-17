#ifndef NIKONOVA_NOVA_BITGL_INCLUDE_COLOR_UTILS_HPP
#define NIKONOVA_NOVA_BITGL_INCLUDE_COLOR_UTILS_HPP






#include "Vec.hpp"






constexpr auto to_colorref(Vec3<std::uint8_t> const &color) -> COLORREF;






#include "color_utils.ipp"






#endif