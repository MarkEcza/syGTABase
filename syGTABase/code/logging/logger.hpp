#pragma once
#include <memory>
#include <string>
#include <fstream>
#include <shared_mutex>
#include <windows.h>

namespace sy
{
	enum class logClass {
		GENERAL = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, 
		ALERT = FOREGROUND_RED | FOREGROUND_GREEN,
		FATAL = FOREGROUND_RED,
	};

	class logger {
	public:
		explicit logger(); 
		~logger();

		void logNow(std::string message, logClass type);

	private:
		std::ofstream m_Console;
		std::ofstream m_File;
		std::shared_mutex m_Mutex;
	};
}