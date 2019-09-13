#pragma once
#include "Drawable.h"

namespace sb {
	class Scene;
	class Node : public Drawable {
		int _drawLayer;
		std::vector<Node*> _children;
	public:
		inline Node()
			: _drawLayer(0)
		{ }
		inline virtual ~Node() {
			for (size_t i = 0; i < _children.size(); i++)
				delete _children[i];
		}
		inline const std::vector<Node*>& getChildren() const { return _children; }
		inline int getDrawLayer() const { return _drawLayer; }
		inline void setDrawLayer(int drawLayer) { _drawLayer = drawLayer; }
		inline virtual void update(Scene& scene) { };
		inline virtual void draw(DrawTarget& target, sb::DrawStates drawStates = DrawStates::getDefault()) { }
		template <class T>
		T& createChild() {
			T* child = new T();
			_children.push_back(child);
			return *child;
		}
	};
}