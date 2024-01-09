#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <functional>
#include <vector>

constexpr int SWAPCHAIN_RESIZE_BUFFERS_VTABLE_INDEX = 13;
constexpr int SWAPCHAIN_PRESENT_VTABLE_INDEX = 8;
constexpr int SWAPCHAIN_NUM_FUNCS = 19;

namespace sy
{
	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		void onPresentTick();
		void wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		void renderThis(std::function<void()> render);
	public:
		WNDPROC m_WndProc;
		Microsoft::WRL::ComPtr<ID3D11Device> m_D3dDevice;
		HWND m_Hwnd;
		ImFont* m_DefaultFont;
		bool m_IsVisible;
		void* m_SwapChainPresentAddr;
		void* m_SwapChainResizeAddr;
		std::vector<std::function<void()>> m_ToRender;
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_DxgiSwapchain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_D3dDeviceContext;
	};
}