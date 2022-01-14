#include "Plane.h"

void SimplePhysicsEngine::Plane::buildVertices()
{
	utils::Vector3 lu();
	utils::Vector3 ld(-width * 0.5f, 0, -height * 0.5f);
	utils::Vector3 ru(width * 0.5f, 0, height * 0.5f);
	utils::Vector3 rd(width * 0.5f, 0, -height * 0.5f);

	clearArrays();

	addVertex(-width * 0.5f, 0, height * 0.5f); //lu
	addVertex(-width * 0.5f, 0, -height * 0.5f); //ld
	addVertex(width * 0.5f, 0, height * 0.5f); //ru
	addVertex(width * 0.5f, 0, -height * 0.5f); //rd

	addIndices(0, 1, 2);
	addIndices(2, 1, 3);
	//lu->ld->ru / ru->ld->rd

	addNormal(0, 1.0f ,0);
	addNormal(0, 1.0f, 0);
	addNormal(0, 1.0f, 0);
	addNormal(0, 1.0f, 0);

	buildInterleavedVertices();
}
