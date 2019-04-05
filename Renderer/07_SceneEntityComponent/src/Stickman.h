#pragma once 
#include "DrawTarget.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Quad.h"
#include "Stopwatch.h"

namespace sb {
	class Stickman : public Transformable {
		Quad _chest;
		Quad _head;

		Triangle _rightHand;
		Triangle _leftHand;

		std::vector<Quad> _leftFingers;
		std::vector<Quad> _rightFingers;

	public:
		Stickman()
			: _leftFingers(5), _rightFingers(5)
		{
			_chest.setScale(0.3f, 0.5f);
			_head.setScale(0.2f, 0.2f);
			_head.setPosition(0, 0.4f);
			_leftHand.setScale(0.15f, 0.15f);
			_leftHand.setPosition(-0.3f, 0.1f);
			_rightHand.setScale(0.15f, 0.15f);
			_rightHand.setPosition(0.3f, 0.1f);
			
			initFingers(_leftFingers);
			initFingers(_rightFingers);
		}

		void draw(DrawTarget& drawTarget, DrawBatch& handBatch, Transform parent = Transform::Identity) {
			parent *= getTransform();

			drawTarget.draw(_chest, parent);
			drawTarget.draw(_head, parent);

			handBatch.draw(_leftHand, parent);
			handBatch.draw(_rightHand, parent);

			drawFingers(_leftFingers, handBatch, parent * _leftHand.getTransform(), Vector2f(-1, -0.3f));
			drawFingers(_rightFingers, handBatch, parent * _rightHand.getTransform(), Vector2f(1, -0.3f));
		}

		void update() {
			static Stopwatch sw;
			float dt = sw.getElapsedSeconds();

			_chest.setPosition(0, sinf(dt * 3) * 0.05f);
			_head.setPosition(0, 0.4f + sinf(dt * 10) * 0.02f);
			_leftHand.setPosition(-0.3f + sinf(dt * -5) * 0.05f, 0.1f);
			_rightHand.setPosition(0.3f + sinf(dt * 5) * 0.05f, 0.1f);
		}

	protected:
		void initFingers(std::vector<Quad>& fingers) {
			for (std::size_t i = 0; i < _leftFingers.size(); i++) {
				fingers[i].setScale(1, 0.1f);
				fingers[i].setPosition(0, 0.15f * i);
			}
		}

		void drawFingers(std::vector<Quad>& fingers, DrawBatch& batch, Transform parent, const Vector2f& offset) {
			parent.translate(offset);
			for (std::size_t i = 0; i < fingers.size(); i++)
				batch.draw(fingers[i], parent);
		}

	};
}