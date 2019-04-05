#pragma once
#include "Scene.h"
#include "Quad.h"
#include "Math.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Stickman.h"
#include "Stopwatch.h"
#include "Input.h"

namespace sb 
{
	class Demo1Scene : public Scene
	{
		DrawBatch batch;

		Quad quad1;
		Triangle triangle1;
		Triangle triangle2;
		Stickman stickman;

	public:
		Demo1Scene()
			: batch(1024)
		{
			quad1.setPosition(-0.5f, -0.5f);
			quad1.setRotation(Pi / 8);
			quad1.setScale(0.3f, 0.3f);

			stickman.setPosition(-0.5f, 0.3f);
			stickman.setScale(0.5f, 0.5f);

			triangle1.setPosition(0.5f, 0.3f);
			triangle1.setScale(0.3f, 0.3f);
			triangle1.setRotation(Pi / 5);
			triangle2.setPosition(0.5f, -0.3f);
			triangle2.setScale(0.3f, 0.3f);
			triangle2.setRotation(-Pi / 5);
		}

		void draw(DrawTarget& target, Transform transform = Transform::Identity) {
			target.draw(quad1); 
			stickman.draw(target, batch);
			triangle1.draw(target);
			batch.draw(triangle2);
			target.draw(batch);
		}

		void update() {
			static Stopwatch sw;
			stickman.setRotation(sw.getElapsedSeconds());
			stickman.update();

			if (Input::isKeyGoingDown(KeyCode::Return))
				endScene();
		}
	};
}