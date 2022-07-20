#include <algorithm>



#include "Color.hpp"
#include "Point.hpp"
#include "DrawWindow.hpp"






namespace BitGL
{
	DrawWindow::DrawWindow(Vec2<int> const &size, std::string const &title) :
		Window               (size, title),
		m_colorbuffer        (size[Vec::width] * size[Vec::height], RGB(0, 0, 0)),
		m_colorbuffer_dc     { CreateCompatibleDC(get_hdc()) },
		m_colorbuffer_bitmap { CreateBitmap(size[Vec::width], size[Vec::height], 1, sizeof(decltype(m_colorbuffer)::value_type) * 8, m_colorbuffer.data()) },
		m_old_bitmap         { static_cast<decltype(m_old_bitmap)>(SelectObject(m_colorbuffer_dc, m_colorbuffer_bitmap)) }
	{}
	DrawWindow::~DrawWindow()
	{
		SelectObject(m_colorbuffer_dc, m_old_bitmap);
		DeleteObject(m_colorbuffer_bitmap);
		DeleteDC(m_colorbuffer_dc);
	}



	auto DrawWindow::clear_colorbuffer(ColorRGB const &color) -> void
	{
		std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), to_colorref(color));
	}
	auto DrawWindow::render_point(Point2D const &position, ColorRGB const &color) -> void
	{
		m_colorbuffer[to_1d_index(position, get_size())] = to_colorref(color);
	}
	auto DrawWindow::render_line(Point2D const &point_a, Point2D const &point_b, ColorRGB const &color) -> void
	{
		auto x_0 = point_a[Vec::x];
		auto y_0 = point_a[Vec::y];

		auto x_1 = point_b[Vec::x];
		auto y_1 = point_b[Vec::y];

		auto d_x = std::abs(x_1 - x_0);
		auto s_x = x_1 > x_0 ? 1 : -1;

		auto d_y = -std::abs(y_1 - y_0);
		auto s_y = y_1 > y_0 ? 1 : -1;

		auto error = d_x + d_y;



		while (true)
		{
			render_point({ x_0, y_0 }, color);

			if (x_0 == x_1 && y_0 == y_1) { break; }

			if (error * 2 >= d_y)
			{
				if (x_0 == x_1) { break; }
				error += d_y;
				x_0 += s_x;
			}

			if (error * 2 <= d_x)
			{
				if (y_0 == y_1) { break; }
				error += d_x;
				y_0 += s_y;
			}
		}
	}
	auto DrawWindow::render_linei(int x_0, int y_0, int x_1, int y_1, ColorRGB const &color) -> void
	{
		auto d_x = std::abs(x_1 - x_0);
		auto s_x = x_1 > x_0 ? 1 : -1;

		auto d_y = -std::abs(y_1 - y_0);
		auto s_y = y_1 > y_0 ? 1 : -1;

		auto error = d_x + d_y;



		while (true)
		{
			render_point({ x_0, y_0 }, color);

			if (x_0 == x_1 && y_0 == y_1) { break; }

			if (error * 2 >= d_y)
			{
				if (x_0 == x_1) { break; }
				error += d_y;
				x_0 += s_x;
			}

			if (error * 2 <= d_x)
			{
				if (y_0 == y_1) { break; }
				error += d_x;
				y_0 += s_y;
			}
		}
	}

	auto DrawWindow::render_triangle(Triangle2D const &triangle, ColorRGB const &color) -> void
	{
		render_line(triangle[Vec::x], triangle[Vec::y], color);
		render_line(triangle[Vec::y], triangle[Vec::z], color);
		render_line(triangle[Vec::z], triangle[Vec::x], color);
	}
	// renders clockwise, origin bottom left
	auto DrawWindow::render_rect(Rect2D const& rect, ColorRGB const& color) -> void {
		// bottom line (x, y, x+w, y)
		render_linei(rect[0][0], rect[0][1], rect[0][0] + rect[1][0], rect[0][1], color);
		// right line (x+w, y, x+w, y+h)
		render_linei(rect[0][0] + rect[1][0], rect[0][1], rect[0][0]+rect[1][0], rect[0][1]+rect[1][1], color);
		// top line (x, y+h, x+w, y+h)
		render_linei(rect[0][0], rect[0][1]+rect[1][1], rect[0][0] + rect[1][0], rect[0][1]+rect[1][1], color);
		// left line (x, y, x, y+h)
		render_linei(rect[0][0], rect[0][1], rect[0][0], rect[0][1]+rect[1][1], color);
	}
	auto DrawWindow::render_triangle_filled(Triangle2D triangle, ColorRGB const &color) -> void
	{
		std::sort(triangle.begin(),
		          triangle.end(),
		          [](Point2D const &a, Point2D const &b) { return a[Vec::y] > b[Vec::y]; });

		if (triangle[0][Vec::y] == triangle[1][Vec::y])
		{
			render_triangle_filled_flat_top(triangle, color);
		}
		else if (triangle[1][Vec::y] == triangle[2][Vec::y])
		{
			render_triangle_filled_flat_bottom(triangle, color);
		}
		else
		{
			Point2D divider_point =
			{
				static_cast<int>(triangle[0][Vec::x] + (static_cast<float>(triangle[1][Vec::y] - triangle[0][Vec::y]) / (triangle[2][Vec::y] - triangle[0][Vec::y])) * (triangle[2][Vec::x] - triangle[0][Vec::x])),
				triangle[1][Vec::y]
			};

			render_triangle_filled_flat_top({ triangle[1], divider_point, triangle[2] }, color);
			render_triangle_filled_flat_bottom({ triangle[0], triangle[1], divider_point }, color);
		}
	}
	// Implemented via drawing two triangles
	auto DrawWindow::render_rect_filled(Rect2D rect, ColorRGB const& color) -> void{
		// {{x,y},{x+w,y},{x+w,y+h}} = right side of rect
		Triangle2D right = {{{rect[0][0],rect[0][1]},{rect[0][0]+rect[1][0],rect[0][1]},rect[0]+rect[1]}};
		// {{x,y},{x,y+h},{x+w,y+h}} = left side of rect
		Triangle2D left = {{{rect[0][0],rect[0][1]},{rect[0][0],rect[0][1] + rect[1][1]},rect[0]+rect[1]}};
		render_triangle_filled(right, color);
		render_triangle_filled(left, color);
	};
	
	size_t alib_2d_ar_pos(size_t pitch, size_t x, size_t y, size_t bytes_per_step = 4) {
		return y * pitch + x * bytes_per_step;
	}
	// assumes bpp = 3. Param is unused.
	auto DrawWindow::render_bmp(Point2D const& start, unsigned char* pixels, int w, int h, int bpp) -> void {
		for (int iy = 0; iy < h; iy++) {
			for (int ix = 0; ix < w; ix++) {
				size_t offset = alib_2d_ar_pos(w*3, ix, iy, bpp);
				unsigned char* pixel = pixels + offset;
				m_colorbuffer[to_1d_index(start + Point2D{ix,iy}, get_size())] = RGB(*(pixel), *(pixel+1), *(pixel+2));
			}
		}
	};
	auto DrawWindow::draw() -> void
	{
		SetBitmapBits(m_colorbuffer_bitmap, m_colorbuffer.size() * sizeof(decltype(m_colorbuffer)::value_type), m_colorbuffer.data());
		SelectObject(m_colorbuffer_dc, m_colorbuffer_bitmap);

		auto window_size = get_size();

		BitBlt(get_hdc(),
		       0,
		       0,
		       window_size[Vec::width],
		       window_size[Vec::height],
		       m_colorbuffer_dc,
		       0,
		       0,
		       SRCCOPY);
	}



	auto DrawWindow::render_triangle_filled_flat_top(Triangle2D const &triangle, ColorRGB const &color) -> void
	{
		auto a_x_0 = triangle[2][Vec::x];
		auto a_y_0 = triangle[2][Vec::y];
		auto b_x_0 = triangle[2][Vec::x];
		auto b_y_0 = triangle[2][Vec::y];

		auto a_x_1 = triangle[0][Vec::x];
		auto a_y_1 = triangle[0][Vec::y];

		auto b_x_1 = triangle[1][Vec::x];
		auto b_y_1 = triangle[1][Vec::y];



		auto a_d_x = std::abs(a_x_1 - a_x_0);
		auto a_s_x = a_x_1 > a_x_0 ? 1 : -1;

		auto b_d_x = std::abs(b_x_1 - b_x_0);
		auto b_s_x = b_x_1 > b_x_0 ? 1 : -1;

		auto a_d_y = -(a_y_1 - a_y_0);
		auto a_s_y = 1;

		auto b_d_y = -(b_y_1 - b_y_0);
		auto b_s_y = 1;

		auto a_error = a_d_x + a_d_y;
		auto b_error = b_d_x + b_d_y;



		while (true)
		{
			render_line({ a_x_0, a_y_0 }, { b_x_0, b_y_0 }, color);

			if ((a_x_0 == a_x_1 && a_y_0 == a_y_1) &&
			    (b_x_0 == b_x_1 && b_y_0 == b_y_1)) { break; }

			while (a_error * 2 >= a_d_y)
			{
				if (a_x_0 == a_x_1) { break; }
				a_error += a_d_y;
				a_x_0 += a_s_x;
			}

			if (a_error * 2 <= a_d_x)
			{
				if (a_y_0 == a_y_1) { break; }
				a_error += a_d_x;
				a_y_0 += a_s_y;
			}

			while (b_error * 2 >= b_d_y)
			{
				if (b_x_0 == b_x_1) { break; }
				b_error += b_d_y;
				b_x_0 += b_s_x;
			}

			if (b_error * 2 <= b_d_x)
			{
				if (b_y_0 == b_y_1) { break; }
				b_error += b_d_x;
				b_y_0 += b_s_y;
			}
		}
	}
	auto DrawWindow::render_triangle_filled_flat_bottom(Triangle2D const &triangle, ColorRGB const &color) -> void
	{
		auto a_x_0 = triangle[0][Vec::x];
		auto a_y_0 = triangle[0][Vec::y];
		auto b_x_0 = triangle[0][Vec::x];
		auto b_y_0 = triangle[0][Vec::y];

		auto a_x_1 = triangle[1][Vec::x];
		auto a_y_1 = triangle[1][Vec::y];

		auto b_x_1 = triangle[2][Vec::x];
		auto b_y_1 = triangle[2][Vec::y];



		auto a_d_x = std::abs(a_x_1 - a_x_0);
		auto a_s_x = a_x_1 > a_x_0 ? 1 : -1;

		auto b_d_x = std::abs(b_x_1 - b_x_0);
		auto b_s_x = b_x_1 > b_x_0 ? 1 : -1;

		auto a_d_y = a_y_1 - a_y_0;
		auto a_s_y = -1;

		auto b_d_y = b_y_1 - b_y_0;
		auto b_s_y = -1;

		auto a_error = a_d_x + a_d_y;
		auto b_error = b_d_x + b_d_y;



		while (true)
		{
			render_line({ a_x_0, a_y_0 }, { b_x_0, b_y_0 }, color);

			if ((a_x_0 == a_x_1 && a_y_0 == a_y_1) &&
			    (b_x_0 == b_x_1 && b_y_0 == b_y_1)) { break; }

			while (a_error * 2 >= a_d_y)
			{
				if (a_x_0 == a_x_1) { break; }
				a_error += a_d_y;
				a_x_0 += a_s_x;
			}

			if (a_error * 2 <= a_d_x)
			{
				if (a_y_0 == a_y_1) { break; }
				a_error += a_d_x;
				a_y_0 += a_s_y;
			}

			while (b_error * 2 >= b_d_y)
			{
				if (b_x_0 == b_x_1) { break; }
				b_error += b_d_y;
				b_x_0 += b_s_x;
			}

			if (b_error * 2 <= b_d_x)
			{
				if (b_y_0 == b_y_1) { break; }
				b_error += b_d_x;
				b_y_0 += b_s_y;
			}
		}
	}
}
