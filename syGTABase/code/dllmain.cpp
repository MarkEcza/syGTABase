#include "components.hpp"
#include "misc/time.hpp"
#include <windows.h>

static HANDLE mainThreadHandle{};
static HINSTANCE dllModule{};

namespace sy
{
    DWORD __stdcall main_thread(void*)
    {
        auto loggerInst = std::make_unique<logger>();
        g_Logger->logNow(time::getTime(), logClass::GENERAL);
        auto pointersInst = std::make_unique<pointers>();
        auto rendererInst = std::make_unique<renderer>();
        auto hookingInst = std::make_unique<hooking>();
        auto guiInst = std::make_unique<GUI>();

        while (g_IsRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (GetAsyncKeyState(VK_DELETE))
                g_IsRunning = false;
        }
        
        g_Logger->logNow("Bye bye", logClass::GENERAL);

        guiInst.reset();
        hookingInst.reset();
        rendererInst.reset();
        pointersInst.reset();
        loggerInst.reset();

        CloseHandle(mainThreadHandle);
        FreeLibraryAndExitThread(dllModule, 0);

        return true;
    }
}


BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

        if (hModule)
            DisableThreadLibraryCalls(hModule);

        mainThreadHandle = CreateThread(nullptr, 0, sy::main_thread, nullptr, 0, nullptr);
        dllModule = hModule;
    }

    return TRUE;
}
