#include "components.hpp"

namespace sy::hooks
{
	LRESULT  wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		g_Renderer->wndProc(hwnd, msg, wparam, lparam);

		return getOriginal<decltype(&wndProc)>(g_Pointers->m_WndProc)(hwnd, msg, wparam, lparam);
	}
}