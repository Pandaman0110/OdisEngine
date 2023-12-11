#ifndef ODISLOG_H
#define ODISLOG_H

#include <iostream>
#include <format>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

namespace OdisEngine
{
	enum class LogLevel
	{
		fatal,
		error,
		warning,
		info,
		debug,
	};

	constexpr LogLevel default_filter_level = LogLevel::debug;
	constexpr LogLevel default_log_level = LogLevel::debug;
	const std::string default_log_name = "Log";

	inline std::ostream& operator <<(std::ostream& os, const LogLevel& level)
	{
		switch (level)
		{
		case LogLevel::fatal:
			os << "Fatal";
			break;
		case LogLevel::error:
			os << "Error";
			break;
		case LogLevel::warning:
			os << "Warning";
			break;
		case LogLevel::info:
			os << "Info";
			break;
		case LogLevel::debug:
			os << "Debug";
			break;
		}
		//os << static_cast<int>(level);
		return os;
	}

	template <typename T>
	concept Printable = requires (std::ostream & os, T out)
	{
		os << out;
	};

	///Checks whether the OdisEngine::LogLevel passed in is valid.
		/**
		 *
		 * \param level the level the check.
		 * \returns true if the level is less than or equal to the current OdisEngine::LogLevel.
		 */
	constexpr bool valid_level(LogLevel level, LogLevel filter_level)
	{
		return (static_cast<int>(level) <= static_cast<int>(filter_level));
	}

	constexpr std::size_t max_buffer_size = 1024;

	class Logger
	{
	private:

		///map of channel names to the channel instances
		std::vector<Logger> sub_loggers;

		/// the default OdisEngine::LogLevel of the channel.
		LogLevel default_level = default_log_level;

		/// the filter OdisEngine::LogLevel for messages.
		LogLevel filter_level = default_filter_level;

		/// buffer to store logging messages.
		std::stringstream buffer;
	protected:

		/// optional name to be printed with the log message.
		std::string name = "";
	public:
		Logger(std::string name, LogLevel filter_level, LogLevel default_level) :
			name(name),
			filter_level(filter_level),
			default_level(default_level)
		{};

		Logger(std::string name, LogLevel filter_level) :
			Logger(name, filter_level, default_log_level)
		{};

		Logger(std::string name) :
			Logger(name, default_filter_level, default_log_level)
		{};

		Logger() :
			Logger(default_log_name, default_filter_level, default_log_level)
		{};

		template <Printable T>
		std::ostream& operator <<(const T&& arg) { buffer << arg; };

		/// %Log stuff with a OdisEngine::LogLevel and OdisEngine::Printable values.
		/**
		 * this function flushes the buffer.
		 *
		 * \param level the OdisEngine::LogLevel of the log message.
		 * \tparam args comma seperated list of OdisEngine::Printable values.
		 */
		template <Printable...Args>
		void log(LogLevel level, Args&&...args)
		{
			if (valid_level(level, filter_level))
			{
				buffer << "[" << name << "] ";
				buffer << level << " - ";
				((buffer << std::forward<Args>(args) << " "), ...);
				buffer << "\n";
			}

			buffer.flush();
		}

		/// %Log stuff using a format string like std::print or printf
		/**
		 * This function will cause a compile time error if fmt is not a valid std::format_string
		 *
		 * \param level the OdisEngine::LogLevel of the log message.
		 * \param fmt the format string
		 * \tparam args comma seperated list of OdisEngine::Printable values.
		 */
		template <Printable ...Args>
		void logf(LogLevel level, const std::format_string<Args...> fmt, Args&& ...args)
		{
			log(level, std::vformat(fmt.get(), std::make_format_args(std::forward<Args>(args)...)));
		}

		/// %Log stuff with the default OdisEngine::LogLevel and OdisEngine::Printable values.
		/**
		 * Uses the default OdisEngine::LogLevel.
		 * \overload.
		 */
		template <Printable...Args>
		void log(Args&&...args)
		{
			log(default_level, std::forward<Args>(args)...);
		}


		/** %Log stuff with the default OdisEngine::LogLevel and OdisEngine::Printable values.
		 *
		 * Uses the default OdisEngine::LogLevel
		 * \overload.
		 */
		 /*
		 template <Printable ...Args>
		 void logf(Args&& ...args)
		 {
			 logf(default_level, std::forward<Args>(args)...);
		 };
		 */

		 /// sets the current OdisEngine::Logger::filter_level
		constexpr void set_filter(LogLevel level) { filter_level = level; };

		/// sets the current OdisEngine::Logger::default_level
		constexpr void set_default(LogLevel level) { default_level = level; };

		/// accesses the message buffer
		std::streambuf* rdbuf()
		{
			std::vector<std::streambuf*> sub_loggers_buffers;
			if (sub_loggers.size() != 0)
			{
				for (auto& logger : sub_loggers)
					sub_loggers_buffers.push_back(logger.rdbuf());

				for (auto& sub_logger_buffer : sub_loggers_buffers)
					buffer << sub_logger_buffer;
			}
			return buffer.rdbuf();
		};


		////////////////////////


		/** \overload */
		Logger* create(const std::string& name)
		{
			return create(name, default_filter_level, default_log_level);
		}

		/** \overload */
		Logger* create(const std::string& name, LogLevel filter_level)
		{
			return create(name, filter_level, default_log_level);
		}

		/// Creates a sub logger and returns a pointer to it
		/**
		 * If the OdisEngine::Logger already exists, then nothing is done.
		 *
		 * \param name name of the OdisEngine::Logger to create
		 * \param filter_level the filter OdisEngine::LogLevel to construct the OdisEngine::Logger object with
		 * \param default_level the default OdisEngine::LogLevel to construct the OdisEngine::Logger object with
		 * \return a pointer to a OdisEngine::Logger object
		 */
		Logger* create(const std::string& name, LogLevel filter_level, LogLevel default_level)
		{
			sub_loggers.push_back(Logger{ name, filter_level, default_level });
			return get(name);
		}

		/// Gets an already constructed sub logger
		/**
		 * The sub logger should be previously created with OdisEngine::Log::create()
		 *
		 * \param name name of the OdisEngine::Logger to create
		 * \return a pointer to a OdisEngine::Logger object
		 */
		Logger* get(const std::string& name)
		{
			//find the logger with the name in the sub loggers vector
			auto it = std::find_if(sub_loggers.begin(), sub_loggers.end(), [=](Logger& logger) { return logger.name == name; });
			return &(*it);
		}

		/// Deletes a sub logger
		/**
		 * \param name name of the OdisEngine::Logger to create
		 */
		void erase(const std::string& name)
		{
			//erase the logger with the name in the sub loggers vector
			sub_loggers.erase(std::remove_if(sub_loggers.begin(), sub_loggers.end(), [=](Logger& logger) { return logger.name == name; }));
		}


	};
};

extern std::unique_ptr<OdisEngine::Logger> logger;

#endif