#include "MeshCollider.h"

utils::Vector3 MeshCollider::FindFurthestPoint(utils::Vector3 dir) const
{
	utils::Vector3 maxCoord;
	float maxDist = -FLT_MAX;
		
	for (utils::Vector3 vertex : colliderVertices)
	{	
		float distance = utils::Vector3::DotProduct(vertex, dir);		

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
	float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN, minZ = FLT_MAX, maxZ = FLT_MIN;
	for (auto i = 0; i + 2 < count; i += 3)
	{
		auto vertex = utils::Vector3{ vertices[i],vertices[i + 1],vertices[i + 2] };
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
