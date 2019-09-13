#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "DrawBatch.h"
#include "Node.h"

namespace sb {
	class Scene : public DrawTarget {
		typedef std::vector<const Mesh*> Layer;
		typedef std::map<sb::DrawStates, Layer> LayerMap;
		DrawBatch _batch;
		std::vector<Node*> _nodes;
		LayerMap _layers;
		size_t _capacity;
		size_t _queueCount;
	protected:
		void updateRecursively(Node& node);
		void drawRecursively(Node& node, const DrawStates& states);
		bool mustFlush();
		void flush();
		void flush(const std::vector<const Mesh*>& layer, const DrawStates& states);
	public:
		Scene(ImmediateDrawTarget& target, size_t capacity = 8192)
			: _batch(target), _capacity(capacity)
		{ }
		inline static bool compareVertexCount(const Mesh* left, const Mesh* right) { return left->getVertexCount() < right->getVertexCount(); }
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
		void update();
		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawStates& states = DrawStates::getDefault());
		virtual void draw(const Mesh& mesh, const sb::DrawStates &states = sb::DrawStates::getDefault());
		virtual void draw(ImmediateDrawTarget& target, DrawStates drawStates = DrawStates::getDefault());
	};
}