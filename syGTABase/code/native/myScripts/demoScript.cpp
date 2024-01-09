#include "components.hpp"
#include "native/externNatives.hpp"

void sy::demoScript()
{
	while (true)
	{
		if (g_Renderer->m_IsVisible)
		{
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
		}

		script::getCurrent()->wait();
	}

}
