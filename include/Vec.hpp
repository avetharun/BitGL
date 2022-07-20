#ifndef NIKONOVA_NOVA_BITGL_INCLUDE_VEC_HPP
#define NIKONOVA_NOVA_BITGL_INCLUDE_VEC_HPP






#include <Windows.h>



#include <cstddef>



#include <array>






namespace BitGL
{
	template <typename T> constexpr T alib_max(T first, T second) {return (first < second) ? second : first;};
	template <typename T> constexpr T alib_min(T first, T second) {return (first > second) ? second : first;};
	namespace Vec
	{
		constexpr std::size_t x = 0;
		constexpr std::size_t y = 1;
		constexpr std::size_t z = 2;
		constexpr std::size_t w = 3;

		constexpr std::size_t r = 0;
		constexpr std::size_t g = 1;
		constexpr std::size_t b = 2;
		constexpr std::size_t a = 3;

		constexpr std::size_t s = 0;
		constexpr std::size_t t = 1;
		constexpr std::size_t p = 2;
		constexpr std::size_t q = 3;

		constexpr std::size_t width  = 0;
		constexpr std::size_t height = 1;
		constexpr std::size_t depth  = 2;
	}



	template<typename T>
	using Vec2 = std::array<T, 2>;

	template<typename T>
	using Vec3 = std::array<T, 3>;

	template<typename T>
	using Vec4 = std::array<T, 4>;
}






#endif
