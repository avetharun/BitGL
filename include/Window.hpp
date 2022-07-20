#ifndef NIKONOVA_NOVA_BITGL_INCLUDE_WINDOW_HPP
#define NIKONOVA_NOVA_BITGL_INCLUDE_WINDOW_HPP






#include <Windows.h>



#include <chrono>
#include <string>



#include "Vec.hpp"






namespace BitGL
{
	class Window
	{
	public:
		Window(Vec2<int> const &size, std::string const &title) noexcept;
		~Window() noexcept;


	public:
		auto is_open() const noexcept -> bool;

		auto restore()  noexcept -> void;
		auto minimize() noexcept -> void;

	public:
		auto get_size() const noexcept -> Vec2<int>;

		auto get_hwnd() const noexcept -> HWND;
		auto get_hdc()  const noexcept -> HDC;


	public:
		static auto get_time() noexcept -> float;

		static auto poll_events() noexcept -> void;
		static auto wait_events() noexcept -> void;

	private:
		static auto create_window_class(std::string const &name)                                        noexcept -> ATOM;

		static auto client_to_window(Vec2<int> const &client_size)                                      noexcept -> Vec2<int>;
		static auto create_window(Vec2<int> const &size, std::string const &title, bool const *is_open) noexcept -> HWND;


	private:
		static auto CALLBACK m_window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;
		inline static ATOM m_window_class_id = 0;

		inline static auto m_time = 0.0f;
		inline static auto m_previous_time = std::chrono::steady_clock::now();

		Vec2<int> m_size;

		HWND m_window;
		HDC  m_dc;

		bool m_is_open;
	};
}






#endif
