#pragma once

#include "Drawable.h"
#include "Texture.h"

namespace sb
{
	class Sprite : public Drawable
	{
	public:
		Sprite() 
			: m_texture(NULL)
		{ }

		Sprite(Texture& texture)
			: m_texture(&texture)
		{ }

		inline Texture* getTexture() { return m_texture; }

		inline void setTexture(Texture* texture) { m_texture = texture; }

		inline Mesh& getMesh() { return SpriteMesh; }
		
		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault());


	private:
		static Mesh SpriteMesh;

		Texture* m_texture;
	};
}