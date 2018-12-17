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

		~Error() { 
			if (m_condition)
			{
				SDL_Log("%s", m_stream.str().c_str());
				#if defined(WIN32) && defined(DEBUG)
					__debugbreak();
				#else	
					exit(0);
				#endif		
			}
		}

		std::ostream& die() { return m_stream; }

		std::ostream& dieIf(bool condition) { m_condition = condition; return die(); }

	private:

		std::ostringstream m_stream;

		bool m_condition;
	};
}