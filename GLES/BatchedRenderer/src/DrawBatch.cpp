#include "DrawBatch.h"

namespace sb
{
	DrawBatch::~DrawBatch()
	{
		for (std::size_t i = 0; i < m_drawables.size(); i++)
			delete m_drawables[i];
	}
}