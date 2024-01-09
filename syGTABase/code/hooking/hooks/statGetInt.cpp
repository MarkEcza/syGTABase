#include "components.hpp"

namespace sy::hooks
{
	static uint32_t frameCount{};
	BOOL statGetInt(int a1, uint32_t* hash)
	{
		if (frameCount != *g_Pointers->m_FrameCount && g_IsRunning && g_Execute)
		{
			g_Execute->tick();
			frameCount = *g_Pointers->m_FrameCount;
		}

		return getOriginal<decltype(&statGetInt)>(g_Pointers->m_StatGetInt)(a1, hash);
	}
}