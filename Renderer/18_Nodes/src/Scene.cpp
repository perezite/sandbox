#include "Scene.h"
#include <algorithm>

namespace sb {
	void Scene::updateRecursively(Node& node) {
		auto& children = node.getChildren();
		for (size_t i = 0; i < children.size(); i++)
			updateRecursively(*(children[i]));
		node.update(*this);
	}

	void Scene::drawRecursively(Node& node, const DrawStates& states) {
		auto& children = node.getChildren();
		for (size_t i = 0; i < children.size(); i++)
			drawRecursively(*(children[i]), states);
		DrawTarget::draw(node, states);
	}

	bool Scene::mustFlush() {
		return _queueCount > _capacity;
	}

	void Scene::flush()	{
		for (LayerMap::iterator it = _layers.begin(); it != _layers.end(); it++) {
			auto& layer = it->second;
			std::sort(layer.begin(), layer.end(), compareVertexCount);
			flush(it->second, it->first);
		}
		_batch.complete();
		_layers.clear();
	}

	void Scene::flush(const std::vector<const Mesh*>& layer, const DrawStates& states) {
		for (size_t i = 0; i < layer.size(); i++)
			_batch.draw(*layer[i], states);
	}

	void Scene::update() {
		for (size_t i = 0; i < _nodes.size(); i++)
			updateRecursively(*(_nodes[i]));
	}

	void Scene::draw(const std::vector<Vertex>& vertices, const PrimitiveType & primitiveType, const DrawStates & states) {
		SB_ERROR("Will be deleted");
	}

	void Scene::draw(const Mesh& mesh, const sb::DrawStates& states) {
	/*	std::map<sb::DrawStates, std::string> myMap;
		sb::DrawStates myState;
		 auto& value = myMap[myState];*/
		//myMap[myMap] = "hello";
		// auto& test = _layers[states];
		// _layers[states].push_back(&mesh);
		/*_queueCount++;
		if (mustFlush())
			flush();*/
	}

	void Scene::draw(ImmediateDrawTarget& target, DrawStates states) {
		_batch.setTarget(target);
		Mesh::lock(true);
		for (size_t i = 0; i < _nodes.size(); i++)
			drawRecursively(*(_nodes[i]), states);
		if (!_nodes.empty())
			flush();
	}
}
