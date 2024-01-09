#include "components.hpp"
#include <backends/imgui_impl_dx11.h>

namespace sy::hooks
{
    HRESULT swapChainResizeBuffers(IDXGISwapChain* this_, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	 {	
        if (g_Renderer->m_IsVisible && g_IsRunning)
        {
            ImGui_ImplDX11_InvalidateDeviceObjects();

            auto result = getOriginal<decltype(&swapChainResizeBuffers)>(g_Renderer->m_SwapChainResizeAddr)(this_, bufferCount, width, height, newFormat, swapChainFlags);

            if (SUCCEEDED(result))
            {
                ImGui_ImplDX11_CreateDeviceObjects();
            }

            return result;
        }

        return  getOriginal<decltype(&swapChainResizeBuffers)>(g_Renderer->m_SwapChainResizeAddr)(this_, bufferCount, width, height, newFormat, swapChainFlags);
	 }

	HRESULT swapChainPresent(IDXGISwapChain* this_, UINT syncInterval, UINT flags)
	 {
        if (g_IsRunning)
            g_Renderer->onPresentTick();

        return getOriginal<decltype(&swapChainPresent)>(g_Renderer->m_SwapChainPresentAddr)(this_, syncInterval, flags);
   	 }
}