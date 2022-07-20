#include <cmath>
#include <cstdio>



#include <chrono>
#include <iostream>



#include "Vec.hpp"
#include "DrawWindow.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include_private/stb_image.h"

auto main() -> int
{
	stbi_set_flip_vertically_on_load(true);
	BitGL::DrawWindow window({ 800, 800 }, "BitGL");
	window.restore();

	int bpp, w,h;
	unsigned char* pixels = stbi_load("test.bmp", &w, &h, &bpp, 3);
	auto time_before = BitGL::Window::get_time();
	while (window.is_open())
	{
		auto time_now = BitGL::Window::get_time();
		auto delta_time = time_now - time_before;
		time_before = time_now;

		printf("T: %f\tFPS: %f\n", delta_time, 1 / delta_time);
		window.render_bmp({0,0},pixels, w,h,3);
		window.draw();

		BitGL::Window::poll_events();
	}



	return EXIT_SUCCESS;
}
