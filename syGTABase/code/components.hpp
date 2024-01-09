#pragma once
#include <memory>
#include <atomic>

#include "logging/logger.hpp"
#include "pointers/pointers.hpp"
#include "rendering/renderer.hpp"
#include "hooking/hooking.hpp"
#include "ui/uiDemo.hpp"
#include "native/execute.hpp"
#include "native/myScripts/myScripts.hpp"

// I don't know why i made them shared pointers instead of unique.

namespace sy
{
	inline std::atomic_bool g_IsRunning = true;

	inline std::shared_ptr<logger> g_Logger;
	inline std::shared_ptr<pointers> g_Pointers;
	inline std::shared_ptr<renderer> g_Renderer;
	inline std::shared_ptr<hooking> g_Hooking;
	inline std::shared_ptr<GUI> g_GUI;
	inline std::shared_ptr<execute> g_Execute;
	inline std::shared_ptr<fiberPool> g_FiberPool;
}