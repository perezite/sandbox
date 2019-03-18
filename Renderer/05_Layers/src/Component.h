#pragma once
#include "Drawable.h"
#include "DrawTarget.h"
#include "RenderStates.h"

namespace sb
{
	class Entity;

	class Component : public Drawable
	{
	public:
		Component()
			: m_parent(NULL), m_drawDepth(0)
		{ }

		virtual void draw(DrawTarget& target, RenderStates states) { }

		inline Entity* getParent() const { return m_parent; }

		inline float getDrawDepth() const { return m_drawDepth; }

		inline void setParent(Entity* parent) { m_parent = parent; }

		inline void setDrawDepth(float depth) { m_drawDepth = depth; };

	private:
		Entity* m_parent;

		float m_drawDepth;
	};
}