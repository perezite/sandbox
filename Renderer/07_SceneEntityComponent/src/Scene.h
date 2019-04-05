#pragma once
#include "Drawable.h"

namespace sb 
{
	class Scene : public Drawable
	{
		virtual void update() = 0;
	};
}