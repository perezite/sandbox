#include "Stopwatch.h"

namespace sb
{
	namespace renderer2
	{
		#ifdef WIN32

			Stopwatch::Stopwatch()
			{
				LARGE_INTEGER frequency;
				QueryPerformanceFrequency(&frequency);
				m_secondsPerCycle = 1.0 / frequency.QuadPart;

				reset();
			}

			void Stopwatch::reset()
			{
				LARGE_INTEGER start; 
				QueryPerformanceCounter(&start);
				m_start = start.QuadPart;
			}

			float Stopwatch::getElapsedMs()
			{
				return getElapsedSeconds() * 1000.0f;
			}

			float Stopwatch::getElapsedSeconds()
			{
				LARGE_INTEGER current;
				QueryPerformanceCounter(&current);
				LONGLONG cycles = current.QuadPart - m_start;
				return (float)(cycles * m_secondsPerCycle);
			}

		#else	

			Stopwatch::Stopwatch()
			{
				reset();
			}

			void Stopwatch::reset()
			{
				m_startTicks = SDL_GetTicks();
			}

			float Stopwatch::getElapsedMs()
			{
				return  float(SDL_GetTicks() - m_startTicks);
			}

			float Stopwatch::getElapsedSeconds()
			{
				return getElapsedMs() / 1000.0f;
			}

		#endif
	}
}
