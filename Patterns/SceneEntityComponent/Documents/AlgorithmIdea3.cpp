class Quad : public Drawable {
	Mesh _mesh;
public:	
	void draw(DrawTarget& target, DrawState state) {
		state.transform *= getTransform();
		target.draw(_mesh, state);
	}
}

class Mesh {
public:
	static void lock();
	static void unlock();
	...
}

class Window : public DrawTarget {
public:
	void draw(Mesh& mesh, DrawState& state) {
		...
	}
}

class Scene : public DrawTarget, public Drawable {
	typedef std::vector<Drawable*> Layer;
	typedef std::map<DrawState, Layer> LayerMap;
	LayerMap _layers;	
	DrawBatch _batch;
	size_t _bufferCapacity;
	size_t _bufferCount;
	std::vector<Node> _nodes;
public:
	Scene(bufferCapacity = 8192) : 
		_bufferCapacity(bufferCapacity), _bufferCount(0) 
	{ }

	inline bool mustFlush() {
		_bufferCount >= _bufferCapacity;
	}

	template <class T>
	add() {
		...
		T* node = new T();
		_nodes.push_back(node);
	}
	
	virtual void draw(Mesh& mesh, DrawState& state) {
		_layers[state].push_back(mesh);
		_bufferCount++;
		
		if (mustFlush()) 
			flush();
	}
	
	virtual void draw(DrawTarget& target, DrawState state) {
		Mesh::lock();
		
		for (size_t i = 0; i < _nodes.size(); i++)
			draw(_nodes[i], state);
		flush(target);
		
		Mesh::unlock();
	}
	
	void flush(DrawTarget& target) {
		for(LayerMap::iterator it = _layers.begin(); it != _layers.end(); it++) 
			flush(target, it->first, it->second);
		
		_bufferCount = 0;
	}
	
	void flush(DrawTarget& target, DrawState& state, Layer& layer) {
		for (size_t i = 0; i < layer.size(); i++) 
			_batch.draw(layer[i]);
		
		target.draw(_batch);
	}
}

void main() {
	Window window;
	Scene scene(window);

	scene.add<Quad>();
	scene.add<Triangle>();

	while (window.isOpen()) {
		window.update();
		scene.update();
		
		window.clear();
		scene.draw(window);
		window.clear();
	}
}

