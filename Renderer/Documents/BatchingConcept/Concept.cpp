void main() {
	Window window;
	DrawBatch batch;
	
	Paddle paddle;
	
	...
	
	batch.draw(paddle);
	// paddle.draw(batch);
	window.draw(batch);
	// batch.draw(window);
}

class DrawBatch : public DrawTarget, public Drawable {
	...
	
	class Buffer : public DrawTarget {
		
	}	
}

Paddle::draw(DrawTarget& target, RenderStates states) {
	states.transform *= getTransform();
	
	mainStates = states; 
	mainStates.transform *= mainSprite.getTransform();
	target.draw(mainSprite, mainStates);
	// mainSprite.draw(target, mainStates);
	
	leftPropulsionStates = states;
	leftPropulsionStates.transform *= leftPropulsionSprite.getTransform();
	target.draw(leftPropulsionSprite, leftPropulsionStates);	
	// leftPropulsionSprite.draw(target, leftPropulsionStates);	
}

Sprite::draw(DrawTarget& target, RenderStates states) {
	target.draw(mesh.getVertices(), mesh.getPrimitiveType(), states);
}

Batch::draw(Drawable& drawable, RenderStates& states) {
	m_drawCommands.push_back(DrawCommand(drawable, states));
}

Batch::draw(DrawTarget& target, RenderStates& states) {	
	Buffer buffer(target);
	
	foreach(DrawCommand& command: m_drawCommands)
		buffer.draw(command.drawable, command.states);
		// command.drawable.draw(buffer, command.states); 
	
	buffer.flush();
}

Buffer::Buffer(DrawTarget& target) : m_target(target) { }

Buffer::draw(Drawable& drawable, RenderStates& states) {
	drawable.draw(*this, states);
}

Buffer::draw(vector<Vertex>& vertices, PrimitiveType primitiveType, RenderStates& states) {
	if (tooManyVerticesForBatching(vertices))
		m_target.draw(vertices, primitiveType, states);
	
	if (mustFlush(primtiveType, states))
		flush();
	
	insert(vertices, primitiveType); 
}

Window::draw(Drawable& drawable, States& states) {
	drawable.draw(*this, states); 
}