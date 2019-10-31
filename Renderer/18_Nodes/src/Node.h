#pragma once
#include "BaseNode.h"
#include <algorithm>

namespace sb {
	static inline int generateTypeId() {
		static int typeId = -1;
		typeId++;
		return typeId;
	}

	class Scene;
	template <class T>
	class Node : public BaseNode {
	private:
		template <class U>
		inline U* find(BaseNode* current) {
			auto children = current->getChildren();
			for (size_t i = 0; i < children.size(); i++)
				find(children[i]);

			if (current->getTypeId() == U::getStaticTypeId())
				return (U*)current;
		}

		template <class U>
		inline void findAll(BaseNode* current, std::vector<U*>& found) {
			auto& children = current->getChildren();
			for (size_t i = 0; i < children.size(); i++)
				findAll(children[i], found);

			if (current->getTypeId() == U::getStaticTypeId())
				found.push_back((U*)current);
		}
	public:
		inline static int getStaticTypeId() {
			static int typeId = generateTypeId();
			return typeId;
		}
		inline virtual const int getTypeId() const { return getStaticTypeId(); }

		template <class U>
		inline void find() {
			return find<U>(this);
		}

		template <class U>
		inline std::vector<U*> findAll() {
			std::vector<U*> found;
			findAll<U>(this, found);
			std::reverse(found.begin(), found.end());
			return found;
		}
	};
}