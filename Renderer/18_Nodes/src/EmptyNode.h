#pragma once
#include "Node.h"

namespace sb {
	class EmptyNode : public Node<EmptyNode>, public Transformable {
	public:
		inline virtual void draw(DrawTarget& target, DrawStates states) {
			states.transform *= getTransform();
			states.drawLayer = getDrawLayer();
			drawChildren(target, states);
		}
	};
}
