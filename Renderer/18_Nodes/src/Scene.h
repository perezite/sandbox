#pragma once
#include "Drawable.h"
#include "DrawBatch.h"
#include "Node.h"

namespace sb 
{
	class Scene {
		DrawBatch _batch;
		//size_t _capacity;
		//size_t _drawableCount;
		std::vector<Node*> _nodes;
	public:
		Scene(ImmediateDrawTarget& target, size_t capacity = 8192)
			: _batch(target) /*, _capacity(capacity), _drawableCount(0)*/
		{ }
		inline virtual ~Scene() {
			for (int i = _nodes.size() - 1; i >= 0; i--)
				delete _nodes[i];
		}
		template <class T>
		T& create() {
			T* node = new T();
			_nodes.push_back(node);
			return *node;
		}
	};
}