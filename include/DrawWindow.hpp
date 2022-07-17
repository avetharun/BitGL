#ifndef NIKONOVA_NOVA_BITGL_SRC_DRAWWINDOW_HPP
#define NIKONOVA_NOVA_BITGL_SRC_DRAWWINDOW_HPP






#include <string>
#include <vector>



#include "Window.hpp"






class DrawWindow : public Window
{
public:
	DrawWindow(int const width, int const height, std::string const &title);
	~DrawWindow();


public:
	auto clear_framebuffer(COLORREF const color)                      -> void;
	auto render_point(int const x, int const y, COLORREF const color) -> void;

	auto draw() -> void;


private:
	std::vector<COLORREF> framebuffer;
	HDC                   framebuffer_dc;
	HBITMAP               framebuffer_bitmap;

	HBITMAP old_bitmap; // necessary to properly destruct framebuffer_dc
};






#endif
