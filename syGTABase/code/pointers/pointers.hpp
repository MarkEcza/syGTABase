#pragma once
#include <dxgi.h>
#include "gta/nativeContext.hpp"

namespace sy
{
	namespace types
	{
		using getNativeHandler = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
		using fixVectors = void(*)(rage::scrNativeCallContext*);
	}

	class pointers {
	public:
		explicit pointers();
		~pointers();
	public:
		IDXGISwapChain** m_SwapChain{};
		PVOID m_StatGetInt{};
		rage::scrNativeRegistrationTable* m_NativeRegistrationTable{};
		types::getNativeHandler m_GetNativeHandler{};
		types::fixVectors m_FixVectors{};
		uint32_t* m_FrameCount{};
		int64_t** m_ScriptGlobals{};
		PVOID m_WndProc;
	};
}
