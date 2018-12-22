#include "Material.h"

namespace sb
{
	bool operator <(const Material& left, const Material& right)
	{
		if (left.layer < right.layer)
			return true;

		if (left.layer == right.layer)
			if (*left.shader < *right.shader)
				return true;

		return false;
	}
}