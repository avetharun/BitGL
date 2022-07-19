#ifndef NIKONOVA_NOVA_BITGL_INCLUDE_DRAWWINDOW_HPP
#define NIKONOVA_NOVA_BITGL_INCLUDE_DRAWWINDOW_HPP






#include <cstdint>



#include <string>
#include <vector>



#include "Color.hpp"
#include "Point.hpp"
#include "Rect.hpp"
#include "Triangle.hpp"
#include "Window.hpp"






namespace BitGL
{
	class DrawWindow : public Window
	{
	public:
		DrawWindow(Vec2<int> const &size, std::string const &title);
		~DrawWindow();


	public:
		auto clear_colorbuffer(ColorRGB const &color)                                           -> void;
		auto render_point(Point2D const &position, ColorRGB const &color)                       -> void;
		auto render_line(Point2D const &point_a, Point2D const &point_b, ColorRGB const &color) -> void;
		auto render_linei(int x_0, int y_0, int x_1, int y_1, ColorRGB const &color) -> void;
		
		auto render_rect(Rect2D const& rect, ColorRGB const& color) -> void;
		auto render_rect_filled(Rect2D const& rect, ColorRGB const& color) -> void;
		
		auto render_triangle(Triangle2D const &triangle, ColorRGB const &color) -> void;
		auto render_triangle_filled(Triangle2D triangle, ColorRGB const &color) -> void;

		auto draw() -> void;


	private:
		auto render_triangle_filled_flat_top(Triangle2D const &triangle, ColorRGB const &color)    -> void;
		auto render_triangle_filled_flat_bottom(Triangle2D const &triangle, ColorRGB const &color) -> void;


	private:
		std::vector<COLORREF> m_colorbuffer;
		HDC                   m_colorbuffer_dc;
		HBITMAP               m_colorbuffer_bitmap;

		HBITMAP m_old_bitmap; // necessary to properly destruct framebuffer_dc
	};
}






#endif
