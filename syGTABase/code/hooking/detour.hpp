#pragma once
#include "MinHook.h"

namespace sy 
{
	class detour {
	public:
		detour(const char* name, void* ptr, void* detour) : m_Name(name), m_Ptr(ptr), m_Detour(detour), m_Status(true)
		{
			if (MH_CreateHook(m_Ptr, m_Detour, &m_Original) != MH_OK) {
				m_Status = false;
			}
		}

		void enable()
		{
			if (m_Status && MH_QueueEnableHook(m_Ptr) != MH_OK) {
				m_Status = false;
			}
		}

		void disable()
		{
			if (MH_QueueDisableHook(m_Ptr) != MH_OK) {
				m_Status = false;
				return;
			}

			if (MH_RemoveHook(m_Ptr) != MH_OK) {
				m_Status = false;
			}
		}

		template<typename function>
		function getOriginal() const {
			return reinterpret_cast<function>(m_Original);
		}

		void* getOriginal() const {
			return m_Original;
		}

		bool getStatus() const {
			return m_Status;
		}

	private:
		const char* m_Name;
		void* m_Ptr;
		void* m_Detour;
		void* m_Original;
		bool m_Status;
	};
}