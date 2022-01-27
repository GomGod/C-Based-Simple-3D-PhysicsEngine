#include "AABB.h"
#include <algorithm>
#include <iostream>
#include <vector>

bool AABB::TestAABBCollision(const glm::vec3& point)
{
    return (minX < point.x&& point.x < maxX) &&
        (minY < point.y&& point.y < maxY) &&
        (minZ < point.z&& point.z < maxZ);
}

bool AABB::TestAABBCollision(const AABB& box)
{
    if (maxX < box.minX || minX > box.maxX) return false;
    if (maxY < box.minY || minY > box.maxY) return false;
    if (maxZ < box.minZ || minZ > box.maxZ) return false;
    return true;
}

AABB& AABB::operator+(const glm::vec3& pos)
{
    return AABB{ minX + pos.x, maxX + pos.x, minY + pos.y, maxY + pos.y, minZ + pos.z, maxZ + pos.z };
}

AABB& AABB::operator&(const AABB& origin)
{
    return AABB{ origin.minX , origin.maxX , origin.minY , origin.maxY , origin.minZ, origin.maxZ };
}

AABB AABB::ComputeRotatedObjectAABB(glm::quat rot)
{
    AABB ret = AABB(*this);

    glm::mat4 rotMatrix = glm::mat4_cast(rot);
    glm::vec3 vertices[] =
    {
        glm::vec3(minX, minY, minZ),
        glm::vec3(minX, maxY, minZ),
        glm::vec3(minX, minY, maxZ),
        glm::vec3(minX, maxY, maxZ),
        glm::vec3(maxX, minY, minZ),
        glm::vec3(maxX, minY, maxZ),
        glm::vec3(maxX, maxY, minZ),
        glm::vec3(maxX, maxY, maxZ)
    };

    float newMinX = FLT_MAX, newMaxX = FLT_MIN, newMinY = FLT_MAX, newMaxY = FLT_MIN, newMinZ = FLT_MAX, newMaxZ = FLT_MIN;
    for (auto vertex : vertices)
    {
        auto rotVertex = rotMatrix * glm::vec4(vertex, 1.0f);

        if (newMinX > rotVertex.x)
            newMinX = rotVertex.x;
        else if (newMaxX < rotVertex.x)
            newMaxX = rotVertex.x;

        if (newMinY > rotVertex.y)
            newMinY = rotVertex.y;
        else if (newMaxY < rotVertex.y)
            newMaxY = rotVertex.y;

        if (newMinZ > rotVertex.z)
            newMinZ = rotVertex.z;
        else if (newMaxZ < rotVertex.z)
            newMaxZ = rotVertex.z;
    }

    
    ret.minX = newMinX;
    ret.maxX = newMaxX;
    ret.minY = newMinY;
    ret.maxY = newMaxY;
    ret.minZ = newMinZ;
    ret.maxZ = newMaxZ;

    return ret;
}
