#include "components.hpp"
#include "imgui.h"
#include "native/externNatives.hpp"

namespace sy
{
	sy::GUI::GUI()
	{
		g_Renderer->renderThis([]
			{
				ImGui::ShowDemoWindow();
			});

		g_Renderer->renderThis([]
			{
				ImGui::Begin("Demo");

				if (ImGui::Button("Print name"))
				{
					g_FiberPool->queueJob([]
						{
							g_Logger->logNow(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()), logClass::GENERAL);
						});
				}
				
				ImGui::End();
			});

		g_GUI = std::shared_ptr<GUI>(this);
	}

	sy::GUI::~GUI()
	{
		g_Renderer->m_ToRender.clear();
	}
}