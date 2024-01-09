#include "components.hpp"
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <filesystem>
#include <string>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

sy::renderer::renderer() : m_DxgiSwapchain(*g_Pointers->m_SwapChain), m_Hwnd(FindWindowW(L"grcWindow", nullptr)), m_IsVisible(true)
{
	static IDXGISwapChain* swapChain = *g_Pointers->m_SwapChain;
	static void** vft = *(void***)swapChain;

	m_SwapChainPresentAddr = vft[SWAPCHAIN_PRESENT_VTABLE_INDEX];
	m_SwapChainResizeAddr= vft[SWAPCHAIN_RESIZE_BUFFERS_VTABLE_INDEX];

	void* d3dDevice = nullptr;
	HRESULT hr = m_DxgiSwapchain->GetDevice(__uuidof(ID3D11Device), &d3dDevice);

	if (!SUCCEEDED(hr)) {
		throw std::runtime_error("Failed to get D3D device.");
	}

	m_D3dDevice.Attach(static_cast<ID3D11Device*>(d3dDevice));
	m_D3dDevice->GetImmediateContext(m_D3dDeviceContext.GetAddressOf());

	ImGuiContext* ctx = ImGui::CreateContext();
	ImGui_ImplWin32_Init(m_Hwnd);
	ImGui_ImplDX11_Init(m_D3dDevice.Get(), m_D3dDeviceContext.Get());

	std::string fontMsyh = (std::filesystem::path(std::getenv("SYSTEMROOT")) / "Fonts").string();
	if (std::filesystem::exists(fontMsyh + "\\msyh.ttc"))
		fontMsyh += "\\msyh.ttc";
	else
		fontMsyh += "\\msyh.ttf";

	const auto& io = ImGui::GetIO();

	ImFontConfig fontCfg{};
	fontCfg.FontDataOwnedByAtlas = false;
	m_DefaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontMsyh.c_str(), 20.0f, &fontCfg);
	fontCfg.MergeMode = true;
	io.Fonts->AddFontFromFileTTF(fontMsyh.c_str(), 20.5f, &fontCfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromFileTTF(fontMsyh.c_str(), 20.5f, &fontCfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	io.Fonts->Build();

	g_Renderer = std::shared_ptr<renderer>(this);
}

sy::renderer::~renderer()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void sy::renderer::onPresentTick()
{
	if (m_IsVisible)
	{
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}
	else
	{
		ImGui::GetIO().MouseDrawCursor = false;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (const auto& toRender : m_ToRender)
		if (toRender != nullptr)
			toRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void sy::renderer::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KEYUP && wparam == VK_INSERT)
	{
		static POINT cursor_coords{};
		if (m_IsVisible)
		{
			GetCursorPos(&cursor_coords);
		}
		else if (cursor_coords.x + cursor_coords.y != 0)
		{
			SetCursorPos(cursor_coords.x, cursor_coords.y);
		}

		m_IsVisible = !m_IsVisible;
	}

	if (m_IsVisible)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	}
}

void sy::renderer::renderThis(std::function<void()> render)
{
	m_ToRender.push_back(render);
}
