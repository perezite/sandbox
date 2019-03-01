#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace sb
{
	class Window;

	class Drawable
	{

	public:
		Drawable() : 
			m_transformNeedsUpdate(true), m_position(0, 0), m_scale(1, 1), m_rotation(0)
		{ }

		const Transform& getTransform();

		inline void setPosition(float x, float y) { setPosition(sb::Vector2f(x, y)); }

		void setPosition(const sb::Vector2f& position);

		inline void setScale(float x, float y) { setScale(sb::Vector2f(x, y)); }

		void setScale(const sb::Vector2f& scale);

		void setRotation(float radians);

		virtual void draw(Window& window) = 0;

	protected:
		void updateTransform();

	private:
		Mesh m_mesh;

		Transform m_transform;

		bool m_transformNeedsUpdate;

		Vector2f m_position;

		Vector2f m_scale;

		float m_rotation;
	};
}