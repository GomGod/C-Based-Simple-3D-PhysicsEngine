#pragma once
#include<vector>
#include"Vector3.h"

class MeshCollider
{
private:
	
public:	
	std::vector<utils::Vector3> colliderVertices;
	utils::Vector3 FindFurthestPoint(utils::Vector3 dir) const;	

	void BuildColliderVertices(float* vertices, int count);

	MeshCollider& operator() (const MeshCollider& origin)
	{
		auto ret = new MeshCollider;
		ret->colliderVertices = origin.colliderVertices;

		return *ret;
	}

	MeshCollider& operator+ (const utils::Vector3 pos) const
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
};