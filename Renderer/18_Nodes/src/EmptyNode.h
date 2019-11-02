#pragma once
#include "Node.h"

namespace sb {
	class EmptyNode : public Node<EmptyNode> {
	public:
		inline virtual void draw(DrawTarget& target, DrawStates states) {
			drawChildren(target, states);
		}
	};
}
