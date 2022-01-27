#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AABB
{
public:
    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;

    bool TestAABBCollision(const glm::vec3& point);
    bool TestAABBCollision(const AABB& box);
    AABB& operator+ (const glm::vec3& pos);
    AABB& operator& (const AABB& origin);    
    
    AABB ComputeRotatedObjectAABB(glm::quat rot);
};

