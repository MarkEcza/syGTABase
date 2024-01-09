#pragma once
#include "gta/nativeContext.hpp"
#include <unordered_map>

namespace sy
{
	class nativeCallContext : public rage::scrNativeCallContext
	{
	public:
		nativeCallContext();
	private:
		std::uint64_t m_ReturnStack[10];
		std::uint64_t m_ArgStack[100];
	};

	class nativeInvoker
	{
	public:
		explicit nativeInvoker() = default;
		~nativeInvoker() = default;

		void cacheHandlers();

		void beginCall();
		void endCall(rage::scrNativeHash hash);

		template <typename T>
		void pushArg(T&& value)
		{
			m_CallContext.push_arg(std::forward<T>(value));
		}

		template <typename T>
		T& getReturnValue()
		{
			return *m_CallContext.get_return_value<T>();
		}
	private:
		nativeCallContext m_CallContext;
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> m_HandlerCache;
	};

	inline nativeInvoker g_NativeInvoker;
}