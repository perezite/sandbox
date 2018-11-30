#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include "IndexList.h" 

namespace sb
{
	class Renderer
	{
	public:
		void init();

		void add(Drawable* drawable);

		void remove(Drawable* drawable);

		void render();

	protected: 
		void print();

	private:
		IndexList m_indexList;
	};
}