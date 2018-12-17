#pragma once

#include "Drawable.h"
#include "Vertex.h"
#include <vector>
#include <utility> 

namespace sb
{
	class DrawBatch
	{
	public:
		DrawBatch(Shader* shader = Shader::getDefault())
			: m_material(Material(shader)), m_layer(0), m_vertexCount(0)
		{ }

		virtual ~DrawBatch();

		// https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c
		template<typename T, typename... Args>
		inline T* create(Args&&... args)
		{
			T* drawable = new T(args ...);
			drawable->setMaterial(m_material);
			m_drawables.push_back(drawable);
			insert(drawable);
			return drawable;
		}

		inline Drawable* operator[](std::size_t index) { return m_drawables[index]; }

		inline Material& getMaterial() { return m_material; }

		inline std::vector<Drawable*>& getDrawables() { return m_drawables; }

		inline std::vector<GLushort>& getIndices() { return m_indices; }

		inline const int getLayer() const { return m_layer; }

		inline void setShader(Shader* shader) { m_material.shader = shader; }
		
		inline void setLayer(int layer) { m_layer = layer; }

		void calcVertices(std::vector<Vertex>& result);

	protected:
		void insert(Drawable* drawable);

	private:
		Material m_material;

		int m_layer;

		std::vector<Drawable*> m_drawables;

		std::vector<GLushort> m_indices;

		GLushort m_vertexCount;
	};
}