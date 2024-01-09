#include "detour.hpp"
#include "components.hpp"
#include "pointers/scanner/scanner.hpp"



namespace sy
{
	sy::hooking::hooking()
	{
		MH_Initialize();

		createAndEnableHook("SwapChain_Resize", g_Renderer->m_SwapChainResizeAddr, &hooks::swapChainResizeBuffers);
		createAndEnableHook("SwapChain_Present", g_Renderer->m_SwapChainPresentAddr, &hooks::swapChainPresent);
		createAndEnableHook("wndProc", g_Pointers->m_WndProc, &hooks::wndProc);
		createAndEnableHook("statGetInt", g_Pointers->m_StatGetInt, &hooks::statGetInt);
		createAndEnableHook("setCursorPos", _module("user32.dll").get_export("SetCursorPos").as<void*>(), &hooks::setCursorPos);

		MH_ApplyQueued();

		g_Hooking = std::shared_ptr<hooking>(this);
	}

	hooking::~hooking()
	{
		for (const auto& [addr, hook] : g_Detours)
			hook->disable();
	}
}