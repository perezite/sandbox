#pragma once
#include "DrawStates.h"
#include "Mesh.h"

namespace sb
{
	struct DrawCommand {
		DrawCommand(const Mesh& mesh_, const DrawStates& drawStates_)
			: mesh(&mesh_), drawStates(drawStates_)
		{ }

		const Mesh* mesh;
		DrawStates drawStates;
	};
}