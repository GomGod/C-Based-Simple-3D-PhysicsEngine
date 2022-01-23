#pragma once
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"AABB.h"

class MeshCollider
{	
public:	
	AABB aabb = AABB();
	std::vector<glm::vec3> colliderVertices;
	glm::vec3 FindFurthestPoint(glm::vec3 dir) const;
	void BuildColliderVertices(float* vertices, int count);

	MeshCollider& operator() (const MeshCollider& origin);
	MeshCollider& operator+ (const glm::vec3& pos) const;	
};