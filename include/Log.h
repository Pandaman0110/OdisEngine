#ifndef ODISLOG_H
#define ODISLOG_H

#include <iostream>
#include <map>
#include <format>


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
		 * \param level the OdisEngine::LogLevel of the log message.
		 * \tparam args comma seperated list of OdisEngine::Printable values.
		 */
		template <Printable...Args>
		void log(LogLevel level, Args&&...args)
		{
			if (valid_level(level, filter_level))
			{
				std::cout << name << ": ";
				std::cout << level << " - ";
				((std::cout << std::forward<Args>(args) << " "), ...);
				std::cout << std::endl;
			}
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

	

		/// sets the current OdisEngine::Channel::filter_level
		constexpr void set_filter(LogLevel level) { filter_level = level; };

		/// sets the current OdisEngine::Channel::default_level
		constexpr void set_default(LogLevel level) { default_level = level; };
	};

	/// An object to manage a collection of OdisEngine::Channel
	/**
	 * One of these bad boys is extern declared in Log.h, make sure to define this value before anything else.
	 * There should probably only be one of these.
	 * This object handles the memory for the Channels, you do NOT have to manage the Channels memory.
	 */
	class Log
	{
	private:
		///map of channel names to the channel instances
		std::map<std::string, std::unique_ptr<Channel>> channels;
		
		/// the default OdisEngine::LogLevel of the channel.
		LogLevel default_level = DEFAULT_LOG_DEFAULT_LEVEL;

		/// the filter OdisEngine::LogLevel for messages.
		LogLevel filter_level = DEFAULT_LOG_FILTER_LEVEL;

		std::string name = "Log";


	public:
		Log()
		{
			create("FileSystem");
		};

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
			if (valid_level(level, filter_level))
			{
				std::cout << name << ": ";
				std::cout << level << " - ";
				((std::cout << std::forward<Args>(args) << " "), ...);
				std::cout << std::endl;
			}
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


		/** \overload */
		Channel* create(const std::string& channel_name)
		{
			channels.insert({ channel_name, std::make_unique<Channel>(channel_name) });
			return get(channel_name);
		}

		/** \overload */
		Channel* create(const std::string& channel_name, LogLevel filter_level)
		{
			channels.insert({ channel_name, std::make_unique<Channel>(channel_name, filter_level) });
			return get(channel_name);
		}

		/// Creates a channel and returns a pointer to it
		/**
		 * If the OdisEngine::Channel already exists, then nothing is done.
		 * 
		 * \param channel_name name of the OdisEngine::Channel to create
		 * \param filter_level the filter OdisEngine::LogLevel to construct the OdisEngine::Channel object with
		 * \param default_level the default OdisEngine::LogLevel to construct the OdisEngine::Channel object with
		 * \return a pointer to a OdisEngine::Channel object
		 */
		Channel* create(const std::string& channel_name, LogLevel filter_level, LogLevel default_level)
		{
			channels.insert({ channel_name, std::make_unique<Channel>(channel_name, filter_level, default_level) });
			return get(channel_name);
		}

		/// Gets an already constructed channel
		/**
		 * The channel should be previously created with OdisEngine::Log::create()
		 * 
		 * \param channel_name name of the OdisEngine::Channel to create
		 * \return a pointer to a OdisEngine::Channel object
		 */
		Channel* get (const std::string& channel_name) const
		{
			return channels.at(channel_name).get();
		}

		/// Deletes a channel
		/**
		 * \param channel_name name of the OdisEngine::Channel to create
		 */
		void delete_channel(const std::string& channel_name)
		{
			channels.erase(channel_name);
		}

		/// sets the current OdisEngine::Channel::filter_level
		constexpr void set_filter(LogLevel level) { filter_level = level; };

		/// sets the current OdisEngine::Channel::default_level
		constexpr void set_default(LogLevel level) { default_level = level; };
	};
}

extern std::unique_ptr<OdisEngine::Log> logger;

#endif