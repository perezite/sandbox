#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "DrawBatch.h"
#include "BaseNode.h"
#include "Stopwatch.h"
#include "DrawCommand.h"
#include "LayerType.h"
#include "EmptyNode.h"
#include "Enumerable.h"

namespace sb {
	class Scene : public DrawTarget {
		typedef Enumerable<DrawCommand> DrawCommands;
		typedef std::map<LayerType, DrawCommands> Layers;
		bool _initialized;
		DrawBatch _batch;
		EmptyNode _root;
		std::vector<const BaseNode*> _nodesToRemove;
		Layers _layers;
		size_t _capacity;
		size_t _numQueued;
		Stopwatch _stopwatch;
		Stopwatch _deltaStopwatch;
		float _seconds;
		float _deltaSeconds;
	protected:
		void init();
		void updateSeconds();
		void updateDeltaSeconds();
		void updateRecursively(BaseNode& node);
		bool mustFlush();
		void flush();
		void flush(const DrawCommands& layer);
		void cleanup();
	public:
		Scene(size_t capacity = 8192)
			: _initialized(false), _capacity(capacity), _numQueued(0), _seconds(0), _deltaSeconds(0)
		{ }
		static bool hasZeroCapacity(const LayerType& layerType, const DrawCommands& commands) { return commands.capacity() == 0; }
		inline float getSeconds() const { return _seconds; }
		inline float getDeltaSeconds() const { return _deltaSeconds; }
		template <class T>
		inline T& create() { return _root.createChild<T>(); }
		template <class T>
		inline std::vector<T*> findAll() { return _root.findAll<T>(); }
		template <class T>
		inline T* find() { return _root.find<T>(); }
		void remove(const BaseNode& node) { _nodesToRemove.push_back(&node); }
		void update();
		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawStates& states = DrawStates::getDefault());
		virtual void draw(const Mesh& mesh, const sb::DrawStates &states = sb::DrawStates::getDefault());
		virtual void draw(ImmediateDrawTarget& target, DrawStates drawStates = DrawStates::getDefault());
	};
}