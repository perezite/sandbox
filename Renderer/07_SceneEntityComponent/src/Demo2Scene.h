#pragma once
#include "Scene.h"
#include "Quad.h"
#include "Stopwatch.h"
#include "Input.h"

namespace sb 
{
	class Demo2Scene : public Scene
	{
		Quad quad;

	public:
		Demo2Scene()
		{
			quad.setScale(0.3f, 0.3f);
		}

		void draw(DrawTarget& target, Transform transform = Transform::Identity) {
			quad.draw(target);
		}

		void update() {
			static Stopwatch sw;
			quad.setRotation(sw.getElapsedSeconds());
			if (Input::isKeyGoingDown(KeyCode::Return))
				endScene();
		}
	};
}