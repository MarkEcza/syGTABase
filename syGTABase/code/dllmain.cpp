#include "components.hpp"
#include "misc/time.hpp"
#include <windows.h>


static HANDLE mainThreadHandle{};
static HINSTANCE dllModule{};

namespace sy
{
    DWORD __stdcall mainThread(void*)
    {
        auto loggerInst = std::make_unique<logger>();
        auto pointersInst = std::make_unique<pointers>();
        auto rendererInst = std::make_unique<renderer>();
        auto hookingInst = std::make_unique<hooking>();
        auto executeInst = std::make_unique<execute>( std::vector 
            { std::make_shared<script>(&demoScript) }
        );
        auto fiberPoolInst = std::make_unique<fiberPool>(10);
        auto guiInst = std::make_unique<GUI>();

        g_Logger->logNow("Injected", logClass::GENERAL);

        while (g_IsRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (GetAsyncKeyState(VK_DELETE))
                g_IsRunning = false;
        }
        
        g_Logger->logNow("Bye bye", logClass::GENERAL);

        executeInst.reset();
        guiInst.reset();
        hookingInst.reset();
        rendererInst.reset();
        pointersInst.reset();
        fiberPoolInst.reset();
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

        mainThreadHandle = CreateThread(nullptr, 0, sy::mainThread, nullptr, 0, nullptr);
        dllModule = hModule;
    }

    return TRUE;
}
