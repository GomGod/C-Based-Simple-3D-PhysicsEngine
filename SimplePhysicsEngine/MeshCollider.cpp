#include "MeshCollider.h"
#include <iostream>
glm::vec3 MeshCollider::FindFurthestPoint(glm::vec3 dir) const
{
	glm::vec3 maxCoord = glm::vec3(0);
	float maxDist = -FLT_MAX;

	for (const glm::vec3& vertex : colliderVertices)
	{
		float distance = glm::dot(vertex, dir);			

		if (distance > maxDist)
		{
			maxDist = distance;
			maxCoord = vertex;
		}
	}

	return maxCoord;
}

void MeshCollider::BuildColliderVertices(float* vertices, int count)
{
	std::vector<glm::vec3>().swap(colliderVertices);

	float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN, minZ = FLT_MAX, maxZ = FLT_MIN;
	
	for (auto i = 0; i + 2 < count; i += 3)
	{
		auto vertex = glm::vec3{ vertices[i],vertices[i + 1],vertices[i + 2] };		
		colliderVertices.push_back(vertex);

		if (minX > vertices[i]) minX = vertex.x;
		if (maxX < vertices[i]) maxX = vertex.x;

		if (minY > vertices[i + 1]) minY = vertex.y;
		if (maxY < vertices[i + 1]) maxY = vertex.y;

		if (minZ > vertices[i + 2]) minZ = vertex.z;
		if (maxZ < vertices[i + 2]) maxZ = vertex.z;
	}

	aabb.minX = minX;
	aabb.minY = minY;
	aabb.minZ = minZ;
	aabb.maxX = maxX;
	aabb.maxY = maxY;
	aabb.maxZ = maxZ;
	aabb.ScaleRoot2();
}

MeshCollider& MeshCollider::operator()(const MeshCollider& origin)
{
	auto ret = new MeshCollider;
	ret->colliderVertices = origin.colliderVertices;

	return *ret;
}

MeshCollider& MeshCollider::operator+(const glm::vec3& pos) const
{
	auto vertices = colliderVertices;
	for (auto& vertex : vertices)
	{
		vertex += pos;
	}
	MeshCollider* ret = new MeshCollider;
	ret->colliderVertices = vertices;

	return *ret;
}

glm::vec3 MeshCollider::Support(const MeshCollider* colliderA, const MeshCollider* colliderB, glm::vec3 dir)
{
	return colliderA->FindFurthestPoint(dir) - colliderB->FindFurthestPoint(-dir);
}