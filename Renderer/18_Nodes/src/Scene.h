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
		Stopwatch _stopwatch;
		Stopwatch _deltaStopwatch;
		float _seconds;
		float _deltaSeconds;
		bool _diagnosticsEnabled;
	protected:
		static bool hasZeroCapacity(const LayerType& layerType, const DrawCommands& commands) { return commands.capacity() == 0; }
		void init();
		void updateTime();
		void removeNodes();
		void flush();
		void flush(const DrawCommands& layer);
		void cleanup();
	public:
		Scene()
			: _initialized(false), _seconds(0), _deltaSeconds(0), _diagnosticsEnabled(false)
		{ }
		inline void enableDiagnostics(bool enable) { _diagnosticsEnabled = enable; }
		inline float getSeconds() const { return _seconds; }
		inline float getDeltaSeconds() const { return _deltaSeconds; }
		inline std::vector<BaseNode*> getNodes() { return _root.getChildren(); }
		template <class T>
		inline T& create() { return _root.createChild<T>(); }
		template <class T>
		inline std::vector<T*> findAll() { return _root.findAll<T>(); }
		template <class T>
		inline T* find() { return _root.find<T>(); }
		void remove(const BaseNode& node) { _nodesToRemove.push_back(&node); }
		void update();
		virtual void draw(const Mesh& mesh, const sb::DrawStates &states = sb::DrawStates::getDefault());
		virtual void draw(ImmediateDrawTarget& target, DrawStates drawStates = DrawStates::getDefault());
		void printDiagnostics();
	};
}