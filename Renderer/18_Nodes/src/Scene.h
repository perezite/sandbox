#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "DrawBatch.h"
#include "BaseNode.h"
#include "Stopwatch.h"
#include "DrawCommand.h"

namespace sb {
	class Scene : public DrawTarget {
		typedef std::vector<DrawCommand> Layer;
		typedef std::map<sb::DrawStates, Layer> Layers;
		bool _initialized;
		DrawBatch _batch;
		std::vector<BaseNode*> _nodes;
		Layers _layers;
		size_t _capacity;
		size_t _numQueued;
		Stopwatch _stopwatch;
		float _deltaSeconds;
	protected:
		void init();
		void updateDeltaSeconds();
		void updateRecursively(BaseNode& node);
		void drawRecursively(BaseNode& node, DrawStates states);
		bool mustFlush();
		void flush();
		void flush(const std::vector<DrawCommand>& layer);
		template <class T>
		inline void collectNodesRecursively(BaseNode& node, std::vector<T*>& collectedNodes) {
			auto children = node.getChildren();
			for (size_t i = 0; i < children.size(); i++) {
				auto child = children[i];
				collectNodesRecursively(*child, collectedNodes);
			}
			
			if (T::getStaticTypeId() == node.getTypeId()) {
				auto nodePointer = &node;
				collectedNodes.push_back((T*)nodePointer);
			}
		}
	public:
		Scene( size_t capacity = 8192)
			: _initialized(false), _capacity(capacity), _numQueued(0), _deltaSeconds(0)
		{ }
		inline static bool compareVertexCount(DrawCommand& left, DrawCommand& right) { return left.mesh->getVertexCount() < right.mesh->getVertexCount(); }
		inline float getDeltaSeconds() const { return _deltaSeconds; }
		inline virtual ~Scene() {
			for (int i = _nodes.size() - 1; i >= 0; i--)
				delete _nodes[i];
		}
		template <class T>
		inline T& create() {
			T* node = new T();
			_nodes.push_back(node);
			return *node;
		}
		template <class T>
		inline std::vector<T*> findNodes() {
			std::vector<T*> collectedNodes;
			for (size_t i = 0; i < _nodes.size(); i++) {
				auto node = _nodes[i];
				collectNodesRecursively(*node, collectedNodes);
			}

			return collectedNodes;
		}
		template <class T>
		inline T* findNode() {
			auto nodes = findNodes<T>();
			return nodes.empty() ? NULL : nodes[0];
		}
		void update();
		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawStates& states = DrawStates::getDefault());
		virtual void draw(const Mesh& mesh, const sb::DrawStates &states = sb::DrawStates::getDefault());
		virtual void draw(ImmediateDrawTarget& target, DrawStates drawStates = DrawStates::getDefault());
	};
}