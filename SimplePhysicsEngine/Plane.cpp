#include "Plane.h"

namespace SimplePhysicsEngine
{
	void SimplePhysicsEngine::Plane::BuildVertices()
	{
		ClearArrays();
		glm::vec3 ulu, uld, uru, urd, dlu, dld, dru, drd;
		ulu = glm::vec3(-width * 0.5f, thickness * 0.5f, height * 0.5f);
		uld = glm::vec3(-width * 0.5f, thickness * 0.5f, -height * 0.5f);
		uru = glm::vec3(width * 0.5f, thickness * 0.5f, height * 0.5f);
		urd = glm::vec3(width * 0.5f, thickness * 0.5f, -height * 0.5f);

		dlu = glm::vec3(-width * 0.5f, -thickness * 0.5f, height * 0.5f);
		dld = glm::vec3(-width * 0.5f, -thickness * 0.5f, -height * 0.5f);
		dru = glm::vec3(width * 0.5f, -thickness * 0.5f, height * 0.5f);
		drd = glm::vec3(width * 0.5f, -thickness * 0.5f, -height * 0.5f);
		glm::vec3 arr[] = { ulu, uld, uru, urd, dlu, dld, dru, drd };


		AddVertex(ulu.x, ulu.y, ulu.z); //lu
		AddVertex(uld.x, uld.y, uld.z); //ld
		AddVertex(uru.x, uru.y, uru.z); //ru
		AddVertex(urd.x, urd.y, urd.z); //rd

		AddVertex(dlu.x, dlu.y, dlu.z); //lu
		AddVertex(dld.x, dld.y, dld.z); //ld
		AddVertex(dru.x, dru.y, dru.z); //ru
		AddVertex(drd.x, drd.y, drd.z); //rd

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


		glm::vec3 normal = glm::normalize(glm::vec3(1, 1, 1));

		AddNormal(-normal.x, normal.y, normal.z);
		AddNormal(-normal.x, normal.y, -normal.z);
		AddNormal(normal.x, normal.y, normal.z);
		AddNormal(normal.x, normal.y, -normal.z);

		AddNormal(-normal.x, -normal.y, normal.z);
		AddNormal(-normal.x, -normal.y, -normal.z);
		AddNormal(normal.x, -normal.y, normal.z);
		AddNormal(normal.x, -normal.y, -normal.z);

		BuildInterleavedVertices();
	}
	
}