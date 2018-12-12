#pragma once

#include "Drawable.h"
#include <vector>
#include <utility> 

namespace sb
{
	class DrawBatch
	{
	public:
		DrawBatch(Shader& shader)
			: m_material(Material(&shader))
		{ }

		DrawBatch()
			: m_material(Material())
		{ }

		virtual ~DrawBatch();

		// https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c
		template<typename T, typename... Args>
		inline T* create(Args&&... args)
		{
			T* drawable = new T(args ...);
			drawable->material = m_material;
			m_drawables.push_back(drawable);

			return drawable;
		}

		inline Material& getMaterial() { return m_material; }

		inline std::vector<Drawable*>& getDrawables() { return m_drawables; }

	private:
		Material m_material;

		std::vector<Drawable*> m_drawables;
	};
}