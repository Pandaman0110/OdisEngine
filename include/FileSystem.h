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

		template <typename Rt>
		Rt load_toml_value(std::filesystem::path path, std::string_view toml_path)
		{
			auto ch = logger->get("OdisEngine");

			toml::parse_result result = toml::parse(load_file(path));

			if (result)
			{
				ch->log(LogLevel::info, "Toml file at", path, "succesfully loaded");
			}
			else
			{
				ch->log("Failed parsing toml file at", path, "error message", result.error());
			}

			auto toml_thing = result.at_path(toml_path).value<Rt>();

			if (toml_thing)
			{
				ch->logf(LogLevel::info, "{} \"{}\" {}", "Toml value at", toml_path, "succesfully loaded");
			}
			else
			{
				ch->logf(LogLevel::warning, "{} \"{}\" {}", "Toml value at", toml_path, "could not be loaded. Check that the requested path is a valid path");
			}

			return toml_thing.value();
		}

		template <typename T>
		std::vector<T> load_toml_array(std::filesystem::path path, std::string_view toml_path)
		{
			auto ch = logger->get("OdisEngine");

			toml::parse_result result = toml::parse(load_file(path));

			if (result)
			{
				ch->log(LogLevel::info, "Toml file at", path, "succesfully loaded");
			}
			else
			{
				ch->log("Failed parsing toml file at", path, "error message", result.error());
			}

			auto toml_thing = result.at_path(toml_path).as_array();

			if (toml_thing and toml_thing->is_homogeneous<T>())
			{
				ch->logf(LogLevel::info, "{} \"{}\" {}", "Toml array at", toml_path, "succesfully loaded");
			}
			else
			{
				ch->logf(LogLevel::warning, "{} \"{}\" {}", "Toml value at", toml_path, "could not be loaded. Check that the requested path is a valid path");
			}

			std::vector<T> container{};
			container.reserve(toml_thing->size());

			for (auto i = toml_thing->begin(); i != toml_thing->end(); i++)
			{
				container.push_back(*i->value<T>());
			}

			return container;
		}

		template <typename T>
		std::map<std::string, T> load_toml_map(std::filesystem::path path, std::string_view toml_path)
		{
			auto ch = logger->get("OdisEngine");

			toml::parse_result result = toml::parse(load_file(path));

			if (result)
			{
				ch->log(LogLevel::info, "Toml file at", path, "succesfully loaded");
			}
			else
			{
				ch->log("Failed parsing toml file at", path, "error message", result.error());
			}

			auto toml_thing = result.at_path(toml_path).as_table();

			if (toml_thing and toml_thing->is_homogeneous<T>())
			{
				ch->logf(LogLevel::info, "{} \"{}\" {}", "Toml map at", toml_path, "succesfully loaded");
			}
			else
			{
				ch->logf(LogLevel::warning, "{} \"{}\" {}", "Toml value at", toml_path, "could not be loaded. Check that the requested path is a valid path");
			}

			std::map<std::string, T> container{};

			for (auto i = toml_thing->begin(); i != toml_thing->end(); i++)
			{
				container.insert({ i->first.str(), *i->second.value<T>() });
			}

			return container;
		}
	};
};

#endif
