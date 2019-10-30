#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include <algorithm>

namespace sb {
	class Scene;

	class BaseNode : public Drawable {
		int _drawLayer;
		std::vector<BaseNode*> _children;
	public:
		BaseNode() :_drawLayer(0) 
		{}
		virtual ~BaseNode();
		inline virtual const int getTypeId() const = 0;
		inline const std::vector<BaseNode*>& getChildren() const { return _children; }
		inline int getDrawLayer() const { return _drawLayer; }
		inline void setDrawLayer(int drawLayer) { _drawLayer = drawLayer; }
		virtual void update(Scene& scene);
		inline virtual void update(float ds) {  }
		inline virtual void draw(DrawTarget& target, sb::DrawStates drawStates = DrawStates::getDefault()) { }
		template <class U>
		inline U& createChild() {
			U* child = new U();
			_children.push_back(child);
			return *child;
		}
		inline void drawChildren(DrawTarget& target, const DrawStates& drawStates) {
			for (size_t i = 0; i < _children.size(); i++)
				_children[i]->draw(target, drawStates);
		}
		inline void clearAllChildren() {
			for (size_t i = 0; i < _children.size(); i++)
				_children[i]->clearAllChildren();

			_children.clear();
		}
		inline void removeChild(BaseNode* child) {
			_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
		}
	};
}