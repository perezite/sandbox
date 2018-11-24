#pragma once

#ifdef WIN32
	#include <windows.h>
#endif

#include <SDL2/SDL.h>

namespace sb
{
	namespace renderer2
	{
		class Stopwatch
		{
		public:
			Stopwatch();

			void reset();

			float getElapsedMs();

			float getElapsedSeconds();

		private:
			#ifdef WIN32
				LONGLONG m_start;
				double m_secondsPerCycle;
			#endif		

			unsigned long m_startTicks;
		};
	}
}