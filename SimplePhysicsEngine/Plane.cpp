#include "Plane.h"

namespace SimplePhysicsEngine
{
	void SimplePhysicsEngine::Plane::BuildVertices()
	{
		ClearArrays();

		AddVertex(-width * 0.5f, thickness * 0.5f, height * 0.5f); //lu
		AddVertex(-width * 0.5f, thickness * 0.5f, -height * 0.5f); //ld
		AddVertex(width * 0.5f, thickness * 0.5f, height * 0.5f); //ru
		AddVertex(width * 0.5f, thickness * 0.5f, -height * 0.5f); //rd

		AddVertex(-width * 0.5f, -thickness * 0.5f, height * 0.5f); //lu
		AddVertex(-width * 0.5f, -thickness * 0.5f, -height * 0.5f); //ld
		AddVertex(width * 0.5f, -thickness * 0.5f, height * 0.5f); //ru
		AddVertex(width * 0.5f, -thickness * 0.5f, -height * 0.5f); //rd

		AddIndices(0, 1, 2);
		AddIndices(2, 1, 3);
		//up

		AddIndices(4, 5, 6);
		AddIndices(6, 5, 7);
		//bot

		//side
		AddIndices(1, 3, 5);
		AddIndices(5, 7, 3);

		AddIndices(7, 6, 3);
		AddIndices(3, 6, 2);

		AddIndices(0, 2, 4);
		AddIndices(4, 6, 2);

		AddIndices(0, 4, 1);
		AddIndices(1, 5, 4);

		AddNormal(0, 1.0f, 0);
		AddNormal(0, 1.0f, 0);
		AddNormal(0, 1.0f, 0);
		AddNormal(0, 1.0f, 0);

		AddNormal(0, -1.0f, 0);
		AddNormal(0, -1.0f, 0);
		AddNormal(0, -1.0f, 0);
		AddNormal(0, -1.0f, 0);

		BuildInterleavedVertices();
	}
}