#include "components.hpp"
#include "pointers/scanner/scanner.hpp"

namespace sy::hooks
{
	BOOL setCursorPos(int x, int y)
	{
		if (g_Renderer->m_IsVisible)
			return true;

		return getOriginal<decltype(&setCursorPos)>(_module("user32.dll").get_export("SetCursorPos").as<void*>())(x, y);
	}
}