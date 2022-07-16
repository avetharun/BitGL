#include "Window.hpp"






Window::Window(int const width, int const height, std::string const &title) noexcept
{
	// Check if window class was already registered
	// If it wasn't, create and register one
	if (m_window_class_id == 0) { m_window_class_id = create_window_class("nikonova-nova | BitGL default window class"); }

	m_window = create_window(width, height, title, &m_is_open);
	m_dc     = GetDC(m_window);

	m_is_open = true;
}
Window::~Window() noexcept
{
	ReleaseDC(m_window, m_dc);
	DestroyWindow(m_window);
	// Don't unregister window class yet, dev might still make another window
}



auto Window::is_open() -> bool
{
	return m_is_open;
}

auto Window::restore()  -> void
{
	ShowWindow(m_window, SW_SHOW);
}
auto Window::minimize() -> void
{
	ShowWindow(m_window, SW_SHOWMINIMIZED);
}

auto Window::poll_events() -> void
{
	MSG message;
	while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}
}
auto Window::wait_events() -> void
{
	WaitMessage();

	poll_events();
}



auto Window::get_hwnd() -> HWND
{
	return m_window;
}
auto Window::get_hdc()  -> HDC
{
	return m_dc;
}



auto Window::create_window_class(std::string const &name) -> ATOM
{
	WNDCLASSA window_class
	{
		.style         = CS_HREDRAW | CS_OWNDC | CS_VREDRAW,
		.lpfnWndProc   = reinterpret_cast<WNDPROC>(&m_window_callback),
		.hInstance     = GetModuleHandleA(nullptr),
		.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)),
		.lpszClassName = name.c_str()
	};

	return RegisterClassA(&window_class);
}
auto Window::create_window(int const width, int const height, std::string const &title, bool const *is_open) -> HWND
{
	auto window = CreateWindowA(MAKEINTATOM(m_window_class_id),
	                            title.c_str(),
	                            WS_CAPTION | WS_OVERLAPPED | WS_SYSMENU,
	                            CW_USEDEFAULT,
	                            CW_USEDEFAULT,
	                            width,
	                            height,
	                            nullptr,
	                            nullptr,
	                            GetModuleHandleA(nullptr),
	                            nullptr);

	// Set HWND's user data to a pointer to parent Window's m_is_open
	SetWindowLongPtrA(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(is_open));

	return window;
}



auto CALLBACK Window::m_window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param) -> LRESULT
{
	switch (message)
	{
	case WM_CLOSE:
		{
			// Get m_is_open associated with closed HWND's parent Window and set it to false
			// Necessary so that user defined main loops will know when to stop
			auto m_is_open = reinterpret_cast<bool *>(GetWindowLongPtrA(window, GWLP_USERDATA));
			*m_is_open = false;
			return 0;
		}

	default:
		{
			return DefWindowProcA(window, message, w_param, l_param);
		}
	}
}