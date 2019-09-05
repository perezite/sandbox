	void onUpdateState(DrawState& state) {
		state.transform *= getTransform();
		state.texture = m_texture;
		state.textureTransform = m_textureTransform;
	}

	void onDraw(DrawTarget& target, DrawState state) {
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitveType(), state);
	}

	void Window::draw(Drawable& drawable, State state) {
		onUpdateState(state);
		onDraw(this*, state);
	}

	void Scene::draw(SceneNode& node) {
		// push all children of node
		// push node
	}

	void Scene::draw(Drawable& drawable, State& state) {
		
	}

	void Drawable::draw(DrawTarget& target, State& state) {
		target.onDraw(this*, state);
	}

	void Window::onDraw(Drawable& drawable, State& state) {
		drawable.beforeDraw(state);
		drawable.onDraw(this*, state);
	}

	void Scene::onDraw(Drawable& drawable, State& state) {
		State state;
		drawable.beforeDraw(state);
		_queue.push_back(state);
	}

	void Sprite::beforeDraw(State& state) {
		state.transform *= getTransform();
		state.texture = _texture;
	}

	void Sprite::onDraw(DrawTarget& target, const State state) {
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
		// target.draw(subSprite, state);
		subSprite.onDraw(target, state);
	}

	void Scene::draw(std::vector<Vertex>& vertices, ..., Drawable* caller) {
		if (caller != _current)
			SB_ERROR("...");
	}

	void Scene::drawScene() {
		foreach(auto node in _nodes) {
			_current = node;
			node.draw(this*, State::getDefault());
		}
	}

	void Sprite::beforeDraw(State& state) {
		state.transform *= getTranform();
		state.texture = _texture;
	}

	void Sprite::draw(Scene& scene, State state) {
		scene.draw(_mesh.getVertices(), ..., state, this);
		Sprite sprite;
		sprite.draw(scene, state);
	}

	void Sprite::draw(State& state) {
		state.transform = getTransform();
		state.texture = _texture;
		getMesh().rotate(45 * toRadian);
	}