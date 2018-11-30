#pragma once
#include <sstream>
#include <iostream>
#include <assert.h>
#include <SDL2/SDL.h>

namespace sb
{
	class Error
	{
	public:
		Error()
			: m_condition(true)
		{ }

		std::ostream& die() { return m_stream; }

		std::ostream& dieIf(bool condition) { 
			m_condition = condition;
			return m_stream;
		}

		~Error() { 
			if (!m_condition)
				return;

			SDL_Log("%s", m_stream.str().c_str());
			#ifdef WIN32
				__debugbreak();
			#else	
				exit(0);
			#endif		
		}

	private:
		std::ostringstream m_stream;

		bool m_condition;
	};
}