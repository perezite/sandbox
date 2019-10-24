#pragma once
#include "BaseNode.h"

namespace sb {
	static inline int generateTypeId() {
		static int typeId = -1;
		typeId++;
		return typeId;
	}

	class Scene;
	template <class T>
	class Node : public BaseNode {
	public:
		inline static int getStaticTypeId() {
			static int typeId = generateTypeId();
			return typeId;
		}
		inline virtual const int getTypeId() const { return getStaticTypeId(); }
		template <class U>
		inline std::vector<U*> findChildren() {
			std::vector<U*> children;
			auto allChildren = getChildren();
			for (size_t i = 0; i < allChildren.size(); i++) {
				if (allChildren[i]->getTypeId() == U::getStaticTypeId())
					children.push_back(allChildren[i]);
			}

			return children;
		}
		template <class U>
		inline U* findChild() {
			auto children = findChildren<U>();
			return children.empty() ? NULL : children[0];
		}
	};
}