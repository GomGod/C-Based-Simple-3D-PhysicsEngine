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
	for (auto i = 0; i+2 < count ; i += 3)
	{		
		colliderVertices.push_back(utils::Vector3{ vertices[i],vertices[i + 1],vertices[i + 2] });
	}
}
