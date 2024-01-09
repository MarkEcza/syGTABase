#include "nativeInvoker.hpp"
#include "components.hpp"
#include "crossmap.hpp"

namespace sy
{
	nativeCallContext::nativeCallContext()
	{
		m_return_value = &m_ReturnStack[0];
		m_args = &m_ArgStack[0];
	}

	void nativeInvoker::cacheHandlers()
	{
		for (const rage::scrNativeMapping& mapping : g_Crossmap)
		{
			rage::scrNativeHandler handler = g_Pointers->m_GetNativeHandler(
				g_Pointers->m_NativeRegistrationTable, mapping.second);

			m_HandlerCache.emplace(mapping.first, handler);
		}
	}

	void nativeInvoker::beginCall()
	{
		m_CallContext.reset();
	}

	void nativeInvoker::endCall(rage::scrNativeHash hash)
	{
		if (auto it = m_HandlerCache.find(hash); it != m_HandlerCache.end())
		{
			rage::scrNativeHandler handler = it->second;

			handler(&m_CallContext);
			g_Pointers->m_FixVectors(&m_CallContext);
		}
		else
		{
			g_Logger->logNow("Failed to get native cache of: " + std::to_string(hash), logClass::GENERAL);
		}
	}
}