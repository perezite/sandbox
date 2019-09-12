#include "Sprite.h"
#include "DrawTarget.h"

namespace sb
{
	Mesh Sprite::SpriteMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 1, 1, 1), Vector2f(0, 0)),
		Vertex(Vector2f( 0.5f, -0.5f), Color(1, 1, 1, 1), Vector2f(1, 0)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(1, 1, 1, 1), Vector2f(0, 1)),
		Vertex(Vector2f( 0.5f,  0.5f), Color(1, 1, 1, 1), Vector2f(1, 1))
	}, PrimitiveType::TriangleStrip);

	Sprite::Sprite()
		: m_mesh(Sprite::SpriteMesh)
	{ }

	Sprite::Sprite(Texture& texture)
		: m_mesh(Sprite::SpriteMesh), m_texture(&texture)
	{ }

	void initMesh() {
	}

	void Sprite::setTexture(Texture& texture)
	{
		const Vector2i size = texture.getSize();
		setTexture(texture, IntRect(0, 0, size.x, size.y));
	}

	void Sprite::setTexture(Texture& texture, const IntRect& textureArea)
	{
		m_texture = &texture; 
		texture.computeTransform(textureArea, m_textureTransform);
	}

	void Sprite::setColor(const Color& color) {
		for (size_t i = 0; i < m_mesh.getVertexCount(); i++)
			m_mesh[i].color = color;
	}

	/*
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
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), state);
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

	*/

	void Sprite::draw(DrawTarget& target, DrawState state)
	{
		state.transform *= getTransform();
		state.texture = m_texture;
		state.textureTransform = m_textureTransform;
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitiveType(), state);

		// subSprite.draw(target, state);
	}
}
