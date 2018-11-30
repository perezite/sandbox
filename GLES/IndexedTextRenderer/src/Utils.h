#pragma once

#include <vector>

namespace sb
{
	class Utils
	{
		template <class T>
		void removeRange(std::vector<T>& v, const std::vector<T>& elems)
		{
			std::vector<T>::iterator it = v.end();
			for (std::size_t i = 0; i < elems.size(); i++) {
				it = std::remove(v.begin(), it, elems[i]);
			}

			v.erase(it, v.end());
		}
	};
}