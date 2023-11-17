#ifndef ODISLOG_H
#define ODISLOG_H

#include <iostream>
#include <map>


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

	constexpr LogLevel DEFAULT_LOG_FILTER_LEVEL = LogLevel::debug;
	constexpr LogLevel DEFAULT_LOG_DEFAULT_LEVEL = LogLevel::debug;

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
	concept Printable = requires (std::ostream& os, T out)
	{
		os << out;
	};

	/**
	 * Channel object to log stuff with
	 * 
	 */
	class Channel
	{
	private:
		/// the default OdisEngine::LogLevel of the channel.
		LogLevel default_level = DEFAULT_LOG_DEFAULT_LEVEL;

		/// the filter OdisEngine::LogLevel for messages.
		LogLevel filter_level = DEFAULT_LOG_FILTER_LEVEL;

		/// the name of the channel.
		std::string name;

		///Checks whether the OdisEngine::LogLevel passed in is valid.
		/**
		 *
		 * \param level the level the check.
		 * \returns true if the level is less than or equal to the current OdisEngine::LogLevel.
		 */
		constexpr bool valid_level(LogLevel level)
		{
			return (static_cast<int>(level) <= static_cast<int>(filter_level));
		}

	public:

		constexpr Channel(std::string name) :
			name(name)
		{};

		constexpr Channel(std::string name, LogLevel filter_level) :
			name(name),
			filter_level(filter_level)
		{};

		constexpr Channel(std::string name, LogLevel filter_level, LogLevel default_level) : 
			name(name),
			filter_level(filter_level), 
			default_level(default_level) 
		{};
		
		/// %Log stuff with a OdisEngine::LogLevel and OdisEngine::Printable values.
		/**
		 * this function flushes the buffer.
		 * 
		 * \param log_level the OdisEngine::LogLevel of the log message.
		 * \tparam args comma seperated list of OdisEngine::Printable values.
		 */
		template <Printable...Args>
		void log(LogLevel level, Args&&...args)
		{
			if (valid_level(level))
			{
				std::cout << name << ": ";
				std::cout << level << " - ";
				((std::cout << std::forward<Args>(args) << " "), ...);
				std::cout << std::endl;
			}
		}

		/// %Log stuff with the default OdisEngine::LogLevel and OdisEngine::Printable values.
		/**
		 * Uses the default OdisEngine::LogLevel.
		 * \overload.
		 */
		template <Printable...Args>
		void log(Args&&...args)
		{
			if (valid_level(default_level))
			{
				std::cout << name << ": ";
				std::cout << default_level << " - ";
				((std::cout << std::forward<Args>(args) << ", "), ...);
				std::cout << std::endl;
			}
		}

		/// sets the current OdisEngine::Channel::filter_level
		constexpr void set_filter(LogLevel level) { filter_level = level; };

		/// sets the current OdisEngine::Channel::default_level
		constexpr void set_default(LogLevel level) { default_level = level; };
	};


	class Log
	{
	private:
		std::map<std::string, std::unique_ptr<Channel>> channels;


	public:
		Log()
		{

		};

		Channel* create_channel(const std::string& channel_name)
		{
			channels.insert({ channel_name, std::make_unique<Channel>(channel_name) });
			return get_channel(channel_name);
		}
		Channel* create_channel(const std::string& channel_name, LogLevel filter_level)
		{
			channels.insert({ channel_name, std::make_unique<Channel>(channel_name, filter_level) });
			return get_channel(channel_name);
		}
		Channel* create_channel(const std::string& channel_name, LogLevel filter_level, LogLevel default_level)
		{
			channels.insert({ channel_name, std::make_unique<Channel>(channel_name, filter_level, default_level) });
			return get_channel(channel_name);
		}

		Channel* get_channel (const std::string& channel_name) const
		{
			return channels.at(channel_name).get();
		}

		void delete_channel(const std::string& channel_name)
		{
			channels.erase(channel_name);
		}
	};
}

extern std::unique_ptr<OdisEngine::Log> logger;

#endif