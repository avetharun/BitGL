#ifndef NIKONOVA_NOVA_BITGL_INCLUDE_COLOR_HPP
#define NIKONOVA_NOVA_BITGL_INCLUDE_COLOR_HPP






#include "Vec.hpp"






namespace BitGL
{
	struct ColorRGB : Vec3<uint8_t> {
	};

	constexpr auto to_colorref(ColorRGB const &color) -> COLORREF;
}






#include "impl/Color.ipp"






#endif
