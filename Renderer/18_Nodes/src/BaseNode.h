#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include "Memory.h"

namespace sb {
	class Scene;

	class BaseNode : public Drawable {
		int _drawLayer;
		std::vector<BaseNode*> _children;
	private:
		void getChildren(BaseNode* current, std::vector<BaseNode*>& allChildren) {
			pushBackRange(allChildren, current->getImmediateChildren());

			auto children = current->getImmediateChildren();
			for (size_t i = 0; i < children.size(); i++)
				getChildren(children[i], allChildren);
		}
	public:
		BaseNode() :_drawLayer(0) 
		{}
		virtual ~BaseNode();
		inline virtual const int getTypeId() const = 0;
		inline const std::vector<BaseNode*>& getImmediateChildren() const { return _children; }
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
		inline void updateChildren(Scene& scene) {
			for (size_t i = 0; i < _children.size(); i++)
				_children[i]->update(scene);
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
		inline void removeImmediateChild(const BaseNode* child) { eraseFromVector(_children, child); }
		inline const std::vector<BaseNode*> getChildren() {
			std::vector<BaseNode*> allChildren;
			for (size_t i = 0; i < _children.size(); i++)
				getChildren(this, allChildren);
			return allChildren;
		}
	};
}