#pragma once

#include "Mesh.h"
#include "Material.h"
#include "GL.h"
#include <vector>

namespace sb
{
	class MeshList
	{
	public:
		MeshList(Shader* shader = Shader::getDefault())
			: m_material(Material(shader)), m_newVertexCount(0), m_newIndexCount(0), m_indexOffset(0), m_needsUpdate(false)
		{ }

		inline Material& getMaterial() { return m_material; }

		std::vector<Vertex>& getVertices();

		std::vector<GLushort>& MeshList::getIndices();

		void add(const Mesh& mesh);

	protected:
		void update();

		void insertMeshes();

		void insertVertices();

		void insertIndices();

	private:
		Material m_material;

		std::vector<Mesh> m_meshes;

		std::vector<Vertex> m_vertices;

		std::vector<GLushort> m_indices;

		GLushort m_indexOffset;

		std::vector<Mesh> m_newMeshes;

		std::size_t m_newVertexCount;

		std::size_t m_newIndexCount;

		bool m_needsUpdate;
	};
}
