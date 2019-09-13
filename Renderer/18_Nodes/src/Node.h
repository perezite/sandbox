#pragma once
#include "Drawable.h"
#include "Transformable.h"

namespace sb {
	class Scene;
	class Node : public Drawable, public Transformable {
		size_t _drawLayer;
		std::vector<Node*> _children;
	public:
		inline virtual ~Node() {
			for (size_t i = 0; i < _children.size(); i++)
				delete _children[i];
		}
		inline std::vector<Node*> getChildren() const { return _children; }
		inline size_t getDrawLayer() const { return _drawLayer; }
		inline void setDrawLayer(size_t drawLayer) { _drawLayer = drawLayer; }
		inline virtual void update(Scene& scene) { };
		inline virtual void draw(DrawTarget& target, sb::DrawState drawStates = DrawState::getDefault()) { }
		template <class T>
		T& createChild() {
			T* child = new T();
			_children.push_back(child);
			return *child;
		}
	};
}