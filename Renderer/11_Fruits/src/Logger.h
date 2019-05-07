#pragma once
#include <sstream>
#include <iostream>
#include <assert.h>
#include <SDL2/SDL.h>

#define NO_ERROR_AS_WARNING_IN_DEBUG_MODE

namespace sb
{
	class NullBuffer : public std::streambuf
	{
	public:
		int overflow(int c) { return c; }
	};

	static NullBuffer NullBuf;
	static std::ostream NullStream(&NullBuf);
}

namespace sb {
	void logMessage(std::stringstream& stream);

	void logError(std::stringstream& stream);

	void logWarning(std::stringstream& stream);
}

#define SB_MESSAGE(stream) do {						\
	std::stringstream out;							\
	out << stream << std::endl;						\
	sb::logMessage(out);							\
} while (false)

#define SB_MESSAGE_IF(condition, stream) do {		\
	if (condition)									\
		SB_MESSAGE(stream);							\
} while (false)

#define SB_ERROR2(stream) do {						\
	std::stringstream out;							\
	out << stream << std::endl;						\
	sb::logError(out);								\
} while (false)

#define SB_ERROR_IF2(condition, stream) do {		\
	if (condition)									\
		SB_ERROR2(stream);							\
} while (false)

#define SB_WARNING2(stream) do {					\
	std::stringstream out;							\
	out << stream << std::endl;						\
	sb::logWarning(out);							\
} while (false)	

#define SB_WARNING_IF2(condition, stream) do {		\
	if (condition)									\
		SB_WARNING2(stream);						\
} while (false)	

#ifndef _DEBUG
	#define SB_DEBUG(stream)			
#else
	#define SB_DEBUG(stream) do {					\
		std::stringstream out;						\
		out << stream << std::endl;					\
		sb::logMessage(out);						\
	} while (false)
#endif

#ifndef _DEBUG
#define SB_DEBUG_IF(condition, stream)			
#else
#define SB_DEBUG_IF(condition, stream) do {			\
		if (condition)								\
			SB_DEBUG(stream);						\
	} while (false)
#endif


#define SB_ERROR() sb::Logger().error()

#define SB_ERROR_IF(condition) (condition ? sb::Logger().error() : sb::NullStream)

#define SB_WARNING() sb::Logger().warning()

#define SB_WARNING_IF(condition) (condition ? sb::Logger().warning() : sb::NullStream)

namespace sb
{
	class Logger
	{
	private:
		enum class LoggerType {
			None, Error, DebugError, Warning
		};

	public:
		Logger() : m_hasError(true), m_hasWarning(true), m_isDebugMode(false), m_type(LoggerType::None) {
			#ifdef _DEBUG
				m_isDebugMode = true;
			#endif
		}

		std::ostream& error() {
			m_type = LoggerType::Error;
			return m_stream;
		}

		std::ostream& errorIf(bool condition) {
			m_type = LoggerType::Error;
			m_hasError = condition;
			return m_stream;
		}

		std::ostream& warning(bool errorInDebugMode = false) {
			m_type = errorInDebugMode && m_isDebugMode ? LoggerType::Error : LoggerType::Warning;
			return m_stream;
		}

		std::ostream& warningIf(bool condition, bool errorInDebugMode = false) {
			m_type = errorInDebugMode && m_isDebugMode ? LoggerType::DebugError : LoggerType::Warning;
			m_hasWarning = condition;
			return m_stream;
		}

		~Logger() {
			bool displayError = m_type == LoggerType::Error || m_type == LoggerType::DebugError;
			if (displayError && m_hasError) {
				std::string prefix = m_type == LoggerType::Error ? "ERROR" : "DEBUG-ERROR";
				SDL_Log("%s: %s", prefix.c_str(), m_stream.str().c_str());
				#ifdef WIN32
					__debugbreak();
				#else	
					exit(0);
				#endif		
			}
			else if (m_type == LoggerType::Warning && m_hasWarning) {
				SDL_Log("WARNING: %s", m_stream.str().c_str());
			}
		}

	private:
		std::ostringstream m_stream;

		bool m_hasError;

		bool m_hasWarning;

		bool m_isDebugMode;

		LoggerType m_type;
	};
}