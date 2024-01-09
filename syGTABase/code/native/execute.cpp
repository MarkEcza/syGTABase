#include "components.hpp"
#include "nativeInvoker.hpp"

namespace sy 
{
	sy::script::script(std::function<void()> func) :
		m_Function(func),
		m_ScriptFiber(nullptr),
		m_MainFiber(nullptr)
	{
		m_ScriptFiber = CreateFiber(0, [](void* param)
			{
				auto thisScript = static_cast<script*>(param);
				thisScript->fiberFunc();
			}, this);
	}

	sy::script::~script()
	{
		if (m_ScriptFiber)
			DeleteFiber(m_ScriptFiber);
	}

	void sy::script::tick()
	{ 
		m_MainFiber = GetCurrentFiber();
		if (!m_WakeTime.has_value() || m_WakeTime.value() <= std::chrono::high_resolution_clock::now())
		{
			SwitchToFiber(m_ScriptFiber);
		}
	}

	void sy::script::wait(std::optional<std::chrono::high_resolution_clock::duration> time)
	{
		if (time.has_value())
		{
			m_WakeTime = std::chrono::high_resolution_clock::now() + time.value();
		}
		else
		{
			m_WakeTime = std::nullopt;
		}

		SwitchToFiber(m_MainFiber);
	}

	script* sy::script::getCurrent()
	{
		return static_cast<script*>(GetFiberData());
	}

	void sy::script::scriptExceptionHandler(PEXCEPTION_POINTERS exp)
	{
		HMODULE mod{};
		DWORD64 offset{};
		char buffer[MAX_PATH]{};
		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)exp->ExceptionRecord->ExceptionAddress, &mod) == TRUE && mod != nullptr)
		{
			offset = ((DWORD64)exp->ExceptionRecord->ExceptionAddress - (DWORD64)mod);
			GetModuleFileNameA(mod, buffer, MAX_PATH - 1);
		}
		g_Logger->logNow(std::string( "Script exception! " ) + buffer, logClass::FATAL);
	}

	void sy::script::fiberFunc()
	{
		__try
		{
			m_Function();
		}
		__except (scriptExceptionHandler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
		{}

		while (true)
		{
			wait();
		}
	}

	execute::execute(std::vector<std::shared_ptr<script>> functions) : m_Functions(functions)
	{
		g_Logger->logNow("Scripts: " + std::to_string(m_Functions.size()), logClass::GENERAL);

		g_Execute = std::shared_ptr<execute>(this);
	}

	execute::~execute()
	{
		m_Functions.clear();
	}

	void execute::tick()
	{
		static bool ensureFiber = (ConvertThreadToFiber(nullptr), true);
		static bool ensureCache = (g_NativeInvoker.cacheHandlers(), true);

		std::lock_guard lock(m_Mutex);

		for (const auto& _script : m_Functions)
			if (_script != nullptr)
				_script->tick();

	}

	void execute::addScript(std::shared_ptr<script> script)
	{
		m_Functions.push_back(script);
	}


	fiberPool::fiberPool(std::size_t numFibers)
	{
		for (std::size_t i = 0; i < numFibers; ++i)
		{
			g_Execute->addScript(std::make_unique<script>(&fiberFunc));
		}

		g_FiberPool = std::shared_ptr<fiberPool>(this);
	}

	fiberPool::~fiberPool()
	{

	}

	void fiberPool::queueJob(std::function<void()> func)
	{
		if (func != nullptr)
		{
			std::lock_guard lock(m_Mutex);
			m_Jobs.push(std::move(func));
		}
	}

	void fiberPool::fiberTick()
	{
		std::unique_lock lock(m_Mutex);
		if (!m_Jobs.empty())
		{
			auto job = std::move(m_Jobs.top());
			m_Jobs.pop();
			lock.unlock();

			std::invoke(std::move(job));
		}
	}

	void fiberPool::fiberFunc()
	{
		while (true)
		{
			g_FiberPool->fiberTick();
			script::getCurrent()->wait();
		}
	}

}

