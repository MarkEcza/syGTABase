#include <windows.h>

#include "components.hpp"
#include "misc/file.hpp"
#include "misc/time.hpp"


namespace sy
{
    sy::logger::logger()
    {
        if (atFolder::exists("Logs\\log.txt") && atFolder::exists("Logs\\Older")) {
            atFolder::moveFile(atFolder::getFile("Logs\\log.txt"), atFolder::getFile("Logs\\Old\\") + "" + time::getTimestamp() + ".txt");
        }
        else {
            atFolder::createFolder("Logs");
            atFolder::createFolder("Logs\\Old");
        }   

        const std::string path = atFolder::getFile("Logs\\log.txt");

        if (!AttachConsole(GetCurrentProcessId()))
            AllocConsole();

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        SetConsoleTitleA("syBase");

        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);

        HWND hwnd = GetConsoleWindow();
        SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hwnd, 0, (255 * 90) / 100, LWA_ALPHA);

        m_File.open(path.c_str(), std::ios_base::out | std::ios_base::app);
        m_Console.open("CONOUT$");

        g_Logger = std::shared_ptr<logger>(this);
    }

    sy::logger::~logger()
    {
        m_Console.close();
        m_File.close();

        FreeConsole();
    }

    void logger::logNow(std::string message, logClass type) {
        const auto& get_type_string = [&](logClass t) {
            switch (t) {
                case logClass::GENERAL:
                    return "General -  ";
                case logClass::ALERT:
                    return "Alert -  ";
                case logClass::FATAL:
                    return "Fatal Error - ";
            }

            return "";
            };

        const auto msg = "[" + time::getTime() + "] " + get_type_string(type) + message;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)type);
        m_Console << msg << std::endl;
        m_File << msg << std::endl;
    }

}
