#include "components.hpp"
#include "scanner/scanner.hpp"

namespace sy
{
	pointers::pointers()
	{
		auto gta = pattern("GTA5.exe");

		m_SwapChain = gta.scanNow("SWPC", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8").getResult().add(3).rip().as<IDXGISwapChain**>();
		m_StatGetInt = gta.scanNow("SGI", "48 8B C4 48 89 58 10 48 89 70 18 57 48 83 EC 30 48 8B F2").getResult().as<PVOID>();
		m_NativeRegistrationTable = gta.scanNow("NATVS", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").getResult().add(3).rip().as<rage::scrNativeRegistrationTable*>();
		m_GetNativeHandler = gta.scanNow("GNH", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").getResult().add(12).rip().as<types::getNativeHandler>();
		m_FixVectors = gta.scanNow("FV", "83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA").getResult().as<types::fixVectors>();
		m_FrameCount = gta.scanNow("FC", "8B 15 ? ? ? ? 41 FF CF").getResult().add(2).rip().as<uint32_t*>();
	    m_WndProc = gta.scanNow("WNDPROC", "48 8B C4 48 89 58 08 4C 89 48 20 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 A1 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 B8").getResult().as<PVOID>();
		
		g_Pointers = std::shared_ptr<pointers>(this);
	}

	pointers::~pointers()
	{

	}
}  