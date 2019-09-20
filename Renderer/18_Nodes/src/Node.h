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
		static int getStaticTypeId();
		const int getTypeId() const;
	};

	template<class T>
	inline int Node<T>::getStaticTypeId() {
		static int typeId = generateTypeId();
		return typeId;
	}

	template<class T>
	inline const int Node<T>::getTypeId() const
	{
		return getStaticTypeId();
	}
}