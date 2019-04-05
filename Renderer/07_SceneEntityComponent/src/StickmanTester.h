#pragma once 
#include "Transformable.h"
#include "DrawBatch.h"
#include "Quad.h"
#include "Stickman.h"

namespace sb {
	class StickmanTester : public Transformable {
		Quad _quad;
		Stickman _stickman;

	public:
		StickmanTester() {
			_quad.setScale(0.4f, 0.4f);
			_stickman.setPosition(0.4f, -0.5f);
		}

		void draw(DrawTarget& target, DrawBatch& stickmanBatch, Transform parent = Transform::Identity) {
			parent *= getTransform();
			target.draw(_quad, parent);
			_stickman.draw(target, stickmanBatch, parent);
		}

		void update() {
			_stickman.update();
		}
	};
}