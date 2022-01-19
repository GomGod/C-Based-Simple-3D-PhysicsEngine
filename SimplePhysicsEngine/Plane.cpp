#include "Plane.h"

namespace SimplePhysicsEngine
{
	void SimplePhysicsEngine::Plane::buildVertices()
	{
		clearArrays();

		addVertex(-width * 0.5f, thickness * 0.5f, height * 0.5f); //lu
		addVertex(-width * 0.5f, thickness * 0.5f, -height * 0.5f); //ld
		addVertex(width * 0.5f, thickness * 0.5f, height * 0.5f); //ru
		addVertex(width * 0.5f, thickness * 0.5f, -height * 0.5f); //rd

		addVertex(-width * 0.5f, -thickness * 0.5f, height * 0.5f); //lu
		addVertex(-width * 0.5f, -thickness * 0.5f, -height * 0.5f); //ld
		addVertex(width * 0.5f, -thickness * 0.5f, height * 0.5f); //ru
		addVertex(width * 0.5f, -thickness * 0.5f, -height * 0.5f); //rd

		addIndices(0, 1, 2);
		addIndices(2, 1, 3);
		//up

		addIndices(4, 5, 6);
		addIndices(6, 5, 7);
		//bot

		//side
		addIndices(1, 3, 5);
		addIndices(5, 7, 3);

		addIndices(7, 6, 3);
		addIndices(3, 6, 2);

		addIndices(0, 2, 4);
		addIndices(4, 6, 2);

		addIndices(0, 4, 1);
		addIndices(1, 5, 4);

		addNormal(0, 1.0f, 0);
		addNormal(0, 1.0f, 0);
		addNormal(0, 1.0f, 0);
		addNormal(0, 1.0f, 0);

		addNormal(0, -1.0f, 0);
		addNormal(0, -1.0f, 0);
		addNormal(0, -1.0f, 0);
		addNormal(0, -1.0f, 0);

		buildInterleavedVertices();

		aabb->minX = -width * 0.5f;
		aabb->maxX = width * 0.5f;
		aabb->minY = -thickness * 0.5f;
		aabb->maxY = thickness * 0.5f;
		aabb->minZ = -height * 0.5f;
		aabb->maxZ = height * 0.5f;
	}
}