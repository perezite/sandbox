#pragma once
#include "Scene.h"
#include "Quad.h"
#include "Math.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Stickman.h"

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
			
			triangle1.setPosition(0.5f, 0.3f);
			triangle2.setPosition(0.5f - 0.3f);
		}

		void draw(sb::DrawTarget &, sb::Transform) {
			
		}

		void update() {

		}
	};
}