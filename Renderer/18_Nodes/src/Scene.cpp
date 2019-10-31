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

		removeFromMap(_layers, hasZeroCapacity);
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

		auto children = _root.getChildren();
		for (size_t i = 0; i < children.size(); i++)
			updateRecursively(*(children[i]));
	}

	void Scene::removeNodes() {
		auto nodes = _root.getChildren();
		for (size_t i = 0; i < nodes.size(); i++) 
			removeNodesRecursively(nodes[i], NULL);

		SB_ERROR_IF(!_nodesToRemove.empty(), "Trying to remove one or more non-existing nodes from a scene");
	}

	void Scene::removeNodesRecursively(BaseNode* current, BaseNode* parent) {
		auto children = current->getChildren();

		for (size_t i = 0; i < children.size(); i++) {
			removeNodesRecursively(children[i], current);
		}

		if (mustRemoveNode(current))
			removeNode(current, parent);
	}

	void Scene::removeNode(BaseNode* nodeToRemove, BaseNode* parent) {
		nodeToRemove->clearAllChildren();
		auto nodes = _root.getChildren();

		if (parent == NULL)
			removeFromVector(nodes, nodeToRemove);
		else
			parent->removeChild(nodeToRemove);

		removeFromVector(_nodesToRemove, (const BaseNode*)nodeToRemove);
	}

	bool Scene::mustRemoveNode(BaseNode* node) {
		return std::find(_nodesToRemove.begin(), _nodesToRemove.end(), node) != _nodesToRemove.end();
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
		auto nodes = _root.getChildren();
		Mesh::lock(true);
		for (size_t i = 0; i < nodes.size(); i++)
			DrawTarget::draw(nodes[i], states);
		if (!nodes.empty())
			flush();
		Mesh::lock(false);
	}
}
