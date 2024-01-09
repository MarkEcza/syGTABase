#pragma once
#include <string>
#include <ctime>
#include <chrono>

namespace sy::time
{
	static const std::string getTimestamp() {
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::tm time_info;
		localtime_s(&time_info, &now);
		std::stringstream ss;
		ss << std::put_time(&time_info, "%Y-%m-%d-%H-%M-%S");
		std::string formatted_date = ss.str();

		return formatted_date;
	}

	static const std::string getTime() {
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::tm time_info;
		localtime_s(&time_info, &now);
		std::stringstream ss;
		ss << std::put_time(&time_info, "%H:%M:%S");
		std::string formatted_time = ss.str();

		return formatted_time;
	}
}