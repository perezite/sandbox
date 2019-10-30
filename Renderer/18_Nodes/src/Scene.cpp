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

	void Scene::updateSeconds() {
		if (!_initialized)
			init();

		_seconds = _stopwatch.getElapsedSeconds();
	}

	void Scene::updateDeltaSeconds() {
		if (!_initialized)
			init();

		_deltaSeconds = _deltaStopwatch.getElapsedSeconds();
		_deltaStopwatch.reset();
	}

	void Scene::updateRecursively(BaseNode& node) {
		auto& children = node.getChildren();
		for (size_t i = 0; i < children.size(); i++)
			updateRecursively(*(children[i]));

		node.update(*this);
	}

	bool Scene::mustFlush() {
		return _numQueued > _capacity;
	}

	void Scene::flush() {
		for (Layers::iterator it = _layers.begin(); it != _layers.end(); it++) {
			flush(it->second);
		}

		_batch.complete();
		cleanup();
		_numQueued = 0;
	}

	void Scene::cleanup() {
		for (Layers::iterator it = _layers.begin(); it != _layers.end(); it++) {
			it->second.adjust_capacity();
			it->second.clear();
		}

		deleteFromMap(_layers, hasZeroCapacity);
	}

	void Scene::flush(const DrawCommands& layer) {
		for (size_t i = 0; i < layer.size(); i++) {
			auto& drawCommand = layer[i];
			_batch.draw(*drawCommand.mesh, drawCommand.drawStates);
		}
	}

	void Scene::update() {
		updateSeconds();
		updateDeltaSeconds();

		removeNodes();

		for (size_t i = 0; i < _nodes.size(); i++)
			updateRecursively(*(_nodes[i]));
	}

	void Scene::removeNodes() {
		std::vector<std::pair<BaseNode*, int>> foundNodes;
		for (size_t i = 0; i < _nodesToRemove.size(); i++) {
			auto result = findNode(*_nodesToRemove[i]);
			SB_ERROR_IF(result.first == NULL, "Trying to delete a non-existing node from a scene");
			foundNodes.push_back(result);
		}

		_nodesToRemove.clear();
	}

	std::pair<BaseNode*, int> Scene::findNode(const BaseNode& nodeToFind) {
		for (size_t i = 0; i < _nodes.size(); i++)
			return findNodeRecursively(*_nodes[i], nodeToFind, 0);

		return std::make_pair((BaseNode*)NULL, 0);
	}

	std::pair<BaseNode*, int> Scene::findNodeRecursively(BaseNode& startNode, const BaseNode& nodeToFind, int depth) {
		if (&startNode == &nodeToFind)
			return std::make_pair(&startNode, depth);

		auto children = startNode.getChildren();
		for (size_t i = 0; i < children.size(); i++) {
			auto result = findNodeRecursively(*children[i], nodeToFind, depth + 1);
			if (result.first != NULL)
				return result;
		}

		return std::make_pair((BaseNode*)NULL, depth);
	}

	void Scene::draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states) {
		SB_ERROR("Will be deleted");
	}
		
	void Scene::draw(const Mesh& mesh, const sb::DrawStates& states) {
		_layers[LayerType(states, mesh.getPrimitiveType())].emplace_back(mesh, states);
		_numQueued++;
		if (mustFlush())
			flush();
	}

	void Scene::draw(ImmediateDrawTarget& target, DrawStates states) {
		_batch.setTarget(target);
		Mesh::lock(true);
		for (size_t i = 0; i < _nodes.size(); i++)
			DrawTarget::draw(_nodes[i], states);
		if (!_nodes.empty())
			flush();
		Mesh::lock(false);
	}
}
