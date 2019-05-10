#pragma once
#include "Vector2f.h"

namespace sb 
{
	class Camera
	{
	public:
		Camera()
			: m_width(1), m_rotation(0)
		{ }

		inline const sb::Vector2f& getPosition() const { return m_position; }

		inline float getWidth() const { return m_width; }

		inline float getRotation() const { return m_rotation; }

		inline void setPosition(const sb::Vector2f& position) { m_position = position; }

		inline void setWidth(float width) { m_width = width; }

		inline void setRotation(float rotation) { m_rotation = rotation; }

		inline void translate(const sb::Vector2f& translate) { m_position += translate; }

		inline void rotate(float angle) { m_rotation += angle; }

	private:
		sb::Vector2f m_position;

		float m_width;

		float m_rotation;
	};
}