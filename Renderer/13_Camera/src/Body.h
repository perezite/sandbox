#include "Vector2f.h"
#include "Transformable.h"

namespace sb
{
	struct Body : public Transformable 
	{
		sb::Vector2f velocity;
	};
}