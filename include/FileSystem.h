#ifndef ODIS_FILE_SYSTEM_H
#define ODIS_FILE_SYSTEM_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <ranges>
#include <algorithm>

#include <toml.hpp>

#include <Log.h>

namespace OdisEngine
{
namespace FileSystem
{
	std::string load_file(std::filesystem::path path)
	{
		std::ifstream file{ path };
		std::stringstream buffer;
		const auto size = std::filesystem::file_size(path);

		file >> buffer.rdbuf();

		file.close();

		return buffer.str();
	}

	template <std::ranges::range Rt, typename ... Args >
	Rt load_toml_as(std::filesystem::path path, Args&& ... args)
	{
		auto ch = logger->get_channel("FileSystem");

		std::vector<std::string> keys { args...};
		std::string toml_path;
		Rt range{};

		std::ranges::for_each(keys, [&](std::string s) { toml_path += s + "."; });
		toml_path.erase(toml_path.end() - 1);

		toml::parse_result result = toml::parse(FileSystem::load_file(path));

		if (result)
		{
			ch->log(LogLevel::info, "Toml file at", path, "succesfully loaded");
		}
		else
		{
			ch->log("Failed parsing toml file at", path, "error message", result.error());
		}

		auto toml_thing = result.at_path(toml_path);

		if (toml_thing)
		{
			ch->log(LogLevel::info, "Toml value at", toml_path, "succefully loaded");
		}
		else
		{
			ch->log(LogLevel::warning, "Toml value at", "\"" + toml_path + "\"", "could not be loaded.Check that the requested path is a valid path");
		}

		//ch->log(LogLevel::)

		//for (auto i = toml_thing->begin(); i != toml_thing->end(); i++)
		//{

		//}
		return range;
	}

};
};

#endif
