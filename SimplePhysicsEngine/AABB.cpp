#include "AABB.h"

void AABB::ScaleRoot2()
{
    auto mul = (1.0f - (sqrt(2.0f) * 0.5f));

    auto xOffset = (maxX - minX) * mul;
    auto yOffset = (maxY - minZ) * mul;
    auto zOffset = (maxY - minZ) * mul;

    maxX += xOffset;
    minX -= xOffset;
    maxY += yOffset;
    minY -= yOffset;
    maxZ += zOffset;
    minZ -= zOffset;
}

bool AABB::TestAABBCollision(const utils::Vector3& point)
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

AABB& AABB::operator+(const utils::Vector3& pos)
{
    return AABB{ minX + pos.x, maxX + pos.x, minY + pos.y, maxY + pos.y, minZ + pos.z, maxZ + pos.z };
}

AABB& AABB::operator&(const AABB& origin)
{
    return AABB{ origin.minX , origin.maxX , origin.minY , origin.maxY , origin.minZ, origin.maxZ };
}
