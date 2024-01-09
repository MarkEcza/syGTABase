#pragma once
#include <functional>
#include <optional>
#include <chrono>
#include <vector>
#include <stack>
#include <mutex>

namespace sy
{
	class script
	{
	public:
		explicit script(std::function<void()> func);
		~script();

		void tick();
		void wait(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);
		static script* getCurrent();
		static void scriptExceptionHandler(PEXCEPTION_POINTERS exp);
	private:
		void fiberFunc();
	private:
		void* m_ScriptFiber;
		void* m_MainFiber;
		std::function<void()> m_Function;
		std::optional<std::chrono::high_resolution_clock::time_point> m_WakeTime;
	};

	class fiberPool
	{
	public:
		explicit fiberPool(std::size_t numFibers);
		~fiberPool() = default;

		void queueJob(std::function<void()> func);

		void fiberTick();
		static void fiberFunc();
	private:
		std::mutex m_Mutex;
		std::stack<std::function<void()>> m_Jobs;
	};
	

	class execute
	{
	public:
		execute(std::vector<std::shared_ptr<script>> functions = {});
		~execute();

		void tick();
		void addScript(std::shared_ptr<script> script);
	private:
		std::vector<std::shared_ptr<script>> m_Functions;
		std::recursive_mutex m_Mutex;
	};
}