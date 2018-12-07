#include "Material.h"

namespace sb
{
	bool operator <(const Material& left, const Material& right)
	{
		return left.shader.GetShaderId() < right.shader.GetShaderId();
	}
}