#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace sb
{
	class DrawTarget;

	class Drawable
	{

	public:
		Drawable() : 
			m_transformNeedsUpdate(true), m_position(0, 0), m_scale(1, 1), m_rotation(0)
		{ }

		virtual ~Drawable() { };

		const Transform& getTransform();

		inline const Vector2f& getPosition() const { return m_position; }

		inline float getRotation() const { return m_rotation; }

		inline const Vector2f& getScale() const { return m_scale; }

		inline void setPosition(float x, float y) { setPosition(sb::Vector2f(x, y)); }

		void setPosition(const sb::Vector2f& position);

		inline void translate(const Vector2f& translation) { setPosition(getPosition() + translation);  }

		inline void translate(float x, float y) { setPosition(getPosition() + Vector2f(x, y)); }

		inline void setScale(float x, float y) { setScale(sb::Vector2f(x, y)); }

		void setScale(const sb::Vector2f& scale);

		void setRotation(float radians);

		virtual void draw(DrawTarget& target, Transform transform) = 0;

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