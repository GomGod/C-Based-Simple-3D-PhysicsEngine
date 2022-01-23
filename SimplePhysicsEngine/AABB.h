#pragma once
#include "Vector3.h"
class AABB
{
public:
    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;

    void ScaleRoot2();
    bool TestAABBCollision(const utils::Vector3& point);
    bool TestAABBCollision(const AABB& box);
    AABB& operator+ (const utils::Vector3& pos);
    AABB& operator& (const AABB& origin);    
};

