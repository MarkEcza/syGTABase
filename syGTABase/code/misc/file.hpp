#pragma once
#include <string>
#include <filesystem>
#include <fileapi.h>

namespace sy::atFolder
{
	static const std::string getMainFolder() {
		try
		{
			std::string path = getenv("appdata") + std::string("\\syBase");
			if (std::filesystem::exists(path))
				return path;

			std::filesystem::create_directory(path);

			return path;
		}
		catch (...)
		{
			return "";
		}
	}

	static const void createFolder(std::string name) {
		try
		{
			if (!std::filesystem::exists(getMainFolder() + "\\" + name))
				std::filesystem::create_directory(getMainFolder() + "\\" + name);
		}
		catch (...)
		{
		}
	}

	static const bool exists(std::string name) {
		return std::filesystem::exists(getMainFolder() + "\\" + name);
	}

	static const bool moveFile(std::string oldPath, std::string newPath) {
		int result = std::rename(oldPath.c_str(), newPath.c_str());
		return (result == 0);
	}

	static const std::string getFile(std::string name) {
		return getMainFolder() + "\\" + name;
	}

	static const bool deleteFolder(std::string name, std::string path = "")  {
		bool status = false;

		if (path.empty()) {
			status = RemoveDirectoryA(std::string(getMainFolder() + "\\" + name).c_str());
		}
		else {
			status = RemoveDirectoryA(std::string(path + "\\" + name).c_str());
		}

		return status;
	}

	static const bool delete_file(std::string name, std::string path = "")  {
		int result;

		if (path.empty()) {
			result = std::remove(std::string(getMainFolder() + "\\" + name).c_str());
		}
		else {
			result = std::remove(std::string(path + "\\" + name).c_str());
		}

		return (result == 0);
	}
}