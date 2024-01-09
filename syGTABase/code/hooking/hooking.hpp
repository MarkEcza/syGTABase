#pragma once

#include <unordered_map>
#include <memory>
#include <Windows.h>
#include <dxgi.h>
#include <string>

#include  "detour.hpp"

//TODO: Make the hooking bullshit better.

namespace sy
{
    namespace hooks
    {
        extern HRESULT swapChainResizeBuffers(IDXGISwapChain* this_, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
        extern HRESULT swapChainPresent(IDXGISwapChain* this_, UINT syncInterval, UINT flags);
        extern LRESULT  wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        extern BOOL         setCursorPos(int x, int y);
        extern BOOL         statGetInt(int a1, uint32_t* hash);
    }


    inline std::unordered_map<void*, std::shared_ptr<detour>> g_Detours;

    template<typename function>
    static function getOriginal(void* addr)
    {
        auto it = g_Detours.find(addr);

        if (it != g_Detours.end())
        {
            const auto original = it->second->getOriginal();

            return reinterpret_cast<function>(original);
        }

        return nullptr;
    }

    template <typename function>
    static void createAndEnableHook(const char* hookName, void* addr, function detourFunction)
    {
        detour* newHook = new detour(hookName, addr, reinterpret_cast<void*>(detourFunction));
        g_Detours.insert(std::make_pair(addr, std::shared_ptr<detour>(newHook)));
        g_Detours[addr]->enable();

        if (!newHook->getStatus())
        {
            std::runtime_error(std::string("Failed to create hook!" + std::string(hookName)).c_str());
        }
    }

    class hooking
    {
    public:
        explicit hooking();
        ~hooking();

        void disableHook(void* hookAddr)
        {
            auto it = g_Detours.find(hookAddr);

            if (it != g_Detours.end())
            {
                it->second->disable();
            }
        }

        size_t getHookCount()
        {
            return g_Detours.size();
        }
    };

}