#include "components.hpp"
#include "imgui.h"

namespace sy
{
	sy::GUI::GUI()
	{
		g_Renderer->renderThis([]
			{
				ImGui::ShowDemoWindow();
			});

		g_GUI = std::shared_ptr<GUI>(this);
	}

	sy::GUI::~GUI()
	{
		g_Renderer->m_ToRender.clear();
	}
}