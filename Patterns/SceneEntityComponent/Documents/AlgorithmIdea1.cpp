
// number 1
while (window.isOpen()) {
	sprite.setPosition(1, 2);
	sprite.draw(window);
}

// number 2
while (window.isOpen()) {
	DrawState state;
	state.transform.move(1, 2);
	sprite.draw(window, state);
}

// number 3
Scene scene;
auto& sprite = scene.add<Sprite);
sprite.setPosition(1, 2);
while(window.isOpen()) {
	scene.draw();
}

// number 3, detail
struct DrawCommand { BaseNode* node, DrawState state  };
typedef vector<DrawCommand> DrawCommands;
typedef std::map<int, DrawCommands> Layers;

scene.draw(Window& target, DrawState& state = DrawState::default()) {
	Layers layers;
	collectStates(*this, state, layers);
	drawNodes(layers);
}

void collectStates(BaseNode& node, DrawState parentState, Layers& layers) {
	auto state = parentState;
	node.updateState(state);

	foreach(auto& child in node.getChildren()) 
		collectStates(child, state, layers);
		
	layers[state.drawLayer].push_back({node, state});
}

void drawNodes(Layers layers) 	
	foreach(auto& layer in layers) {
		foreach(auto& command in layer) {
			auto& node = command.node;
			auto& state = command.state;
			window.draw(node, state);
		}
	}
}

// sprite detail
void updateState(DrawState& state = DrawState::default()) {
	state.texture = _texture;
	state.drawLayer = _drawLayer;
}