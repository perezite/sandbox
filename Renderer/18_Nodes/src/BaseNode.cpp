#include "BaseNode.h"
#include "Scene.h"

namespace sb {
	BaseNode::~BaseNode() {
		for (size_t i = 0; i < _children.size(); i++)
			delete _children[i];
	}

	void BaseNode::draw(DrawTarget& target, sb::DrawStates drawStates) {
		drawChildren(target, drawStates);
	}

	void BaseNode::update(Scene& scene) {
		update(scene.getDeltaSeconds());
		updateChildren(scene);
	}
}
