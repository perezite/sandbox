#include "Scene.h"
#include "DrawStates.h"
#include <algorithm>

namespace sb {
	void Scene::init() {
		_stopwatch.reset();
	}

	void Scene::updateDeltaSeconds() {
		if (!_initialized)
			init();

		_deltaSeconds = _stopwatch.getElapsedSeconds();
		_stopwatch.reset();
	}

	void Scene::updateRecursively(BaseNode& node) {
		auto& children = node.getChildren();
		for (size_t i = 0; i < children.size(); i++)
			updateRecursively(*(children[i]));

		node.update(*this);
	}

	void Scene::drawRecursively(BaseNode& node, const DrawStates& states) {
		auto& children = node.getChildren();
		for (size_t i = 0; i < children.size(); i++)
			drawRecursively(*(children[i]), states);

		DrawTarget::draw(node, states);
	}

	bool Scene::mustFlush() {
		return _numQueued > _capacity;
	}

	void Scene::flush() {
		for (Layers::iterator it = _layers.begin(); it != _layers.end(); it++) {
			auto& layer = it->second;
			std::sort(layer.begin(), layer.end(), compareVertexCount);
			flush(it->second);
		}
		_batch.complete();
		_layers.clear();
		_numQueued = 0;
	}

	void Scene::flush(const std::vector<DrawCommand>& layer) {
		for (size_t i = 0; i < layer.size(); i++) {
			auto& drawCommand = layer[i];
			_batch.draw(*drawCommand.mesh, drawCommand.drawStates);
		}
	}

	void Scene::update() {
		updateDeltaSeconds();

		for (size_t i = 0; i < _nodes.size(); i++)
			updateRecursively(*(_nodes[i]));
	}

	void Scene::draw(const std::vector<Vertex>& vertices, const PrimitiveType & primitiveType, const DrawStates & states) {
		SB_ERROR("Will be deleted");
	}
		
	void Scene::draw(const Mesh& mesh, const sb::DrawStates& states) {
		_layers[states].emplace_back(mesh, states);
		_numQueued++;
		if (mustFlush())
			flush();
	}

	void Scene::draw(ImmediateDrawTarget& target, DrawStates states) {
		_batch.setTarget(target);
		Mesh::lock(true);
		for (size_t i = 0; i < _nodes.size(); i++)
			drawRecursively(*(_nodes[i]), states);
		if (!_nodes.empty())
			flush();
		Mesh::lock(false);
	}
}
