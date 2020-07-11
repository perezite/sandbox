#include "Scene.h"
#include "DrawStates.h"
#include "Memory.h"
#include "Triangle.h"
#include <algorithm>

namespace sb {
	void Scene::init() {
		_stopwatch.reset();
		_deltaStopwatch.reset();
		_initialized = true;
	}

	void Scene::updateTime() {
		if (!_initialized)
			init();

		_seconds = _stopwatch.getElapsedSeconds();
		_deltaSeconds = _deltaStopwatch.getElapsedSeconds();
		_deltaStopwatch.reset();
	}

	void Scene::flush() {
		for (Layers::iterator it = _layers.begin(); it != _layers.end(); it++) {
			flush(it->second);
		}

		_batch.complete();
		cleanup();
	}

	void Scene::cleanup() {
		for (Layers::iterator it = _layers.begin(); it != _layers.end(); it++) {
			it->second.adjust_capacity();
			it->second.clear();
		}

		eraseFromMap(_layers, hasZeroCapacity);
	}

	void Scene::flush(const DrawCommands& layer) {
		for (size_t i = 0; i < layer.size(); i++) {
			auto& drawCommand = layer[i];
			_batch.draw(*drawCommand.mesh, drawCommand.drawStates);
		}
	}

	void Scene::update() {
		updateTime();
		removeNodes();
		_root.update(*this);
	}

	void Scene::removeNodes() {
		if (!_nodesToRemove.empty()) {
			_root.removeChildren(_nodesToRemove);
			_nodesToRemove.clear();
		}
	}

	void Scene::draw(const Mesh& mesh, const sb::DrawStates& states) {
		_layers[LayerType(states, mesh.getPrimitiveType())].emplace_back(mesh, states);
	}

	void Scene::draw(ImmediateDrawTarget& target, DrawStates states) {
		_batch.setTarget(target);
		Mesh::lock(true);
		DrawTarget::draw(_root, states);
		if (_diagnosticsEnabled)
			printDiagnostics();
		flush();
		Mesh::lock(false);
	}

	void Scene::printDiagnostics() {
		std::cout << "Scene diagnostics: " << std::endl;
		std::cout << "\t Layers: " << std::endl;

		size_t nodeCount = 0;
		for (auto it = _layers.begin(); it != _layers.end(); it++) {
			std::cout << "\t\tLayer " << std::distance(_layers.begin(), it) << " has " << it->second.size() << " nodes" << std::endl;
			nodeCount += it->second.size();
		}

		std::cout << "\tNode count total: " << nodeCount << std::endl;
	}

}
