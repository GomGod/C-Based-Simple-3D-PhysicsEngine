#include "Sphere.h"

namespace SimplePhysicsEngine
{
    void Sphere::buildVertices()
    {
        const float PI = acos(-1);

        struct Vertex
        {
            float x, y, z;
        };
        std::vector<Vertex> tmpVertices;

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        float maxX = 0, minX = 0, maxY = 0, minY = 0, maxZ = 0, minZ = 0;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;
            float xz = radius * cosf(stackAngle);
            float y = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                Vertex vertex;
                vertex.x = xz * cosf(sectorAngle);
                vertex.y = y;
                vertex.z = xz * sinf(sectorAngle);

                tmpVertices.push_back(vertex);
                
                if (maxX < vertex.x)
                {
                    maxX = vertex.x;
                }
                if (minX > vertex.x)
                {
                    minX = vertex.x;
                }                

                if (maxY < vertex.y)
                {
                    maxY = vertex.y;
                }
                if (minY > vertex.y)
                {
                    minY = vertex.y;
                }

                if (maxZ < vertex.z)
                {
                    maxZ = vertex.z;
                }
                if (minZ > vertex.z)
                {
                    minZ = vertex.z;
                }
            }
        }

        aabb->minX = minX;
        aabb->maxX = maxX;
        aabb->minY = minY;
        aabb->maxY = maxY;
        aabb->minZ = minZ;
        aabb->maxZ = maxZ;

        clearArrays();        

        Vertex v1, v2, v3, v4;
        std::vector<float> n;

        int i, j, k, vi1, vi2;
        int index = 0;
        for (i = 0; i < stackCount; ++i)
        {
            vi1 = i * (sectorCount + 1);
            vi2 = (i + 1) * (sectorCount + 1);

            for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
            {
                //  v1--v3
                //  |    |
                //  v2--v4
                v1 = tmpVertices[vi1];
                v2 = tmpVertices[vi2];
                v3 = tmpVertices[vi1 + 1];
                v4 = tmpVertices[vi2 + 1];

                if (i == 0)
                {
                    addVertex(v1.x, v1.y, v1.z);
                    addVertex(v2.x, v2.y, v2.z);
                    addVertex(v4.x, v4.y, v4.z);


                    n = getFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                    for (k = 0; k < 3; ++k)
                    {
                        addNormal(n[0], n[1], n[2]);
                    }

                    addIndices(index, index + 1, index + 2);


                    index += 3;
                }
                else if (i == (stackCount - 1))
                {
                    addVertex(v1.x, v1.y, v1.z);
                    addVertex(v2.x, v2.y, v2.z);
                    addVertex(v3.x, v3.y, v3.z);

                    for (k = 0; k < 3; ++k)
                    {
                        addNormal(n[0], n[1], n[2]);
                    }

                    addIndices(index, index + 1, index + 2);

                    index += 3;
                }
                else
                {
                    addVertex(v1.x, v1.y, v1.z);
                    addVertex(v2.x, v2.y, v2.z);
                    addVertex(v3.x, v3.y, v3.z);
                    addVertex(v4.x, v4.y, v4.z);

                    n = getFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                    for (k = 0; k < 4; ++k)
                    {
                        addNormal(n[0], n[1], n[2]);
                    }

                    addIndices(index, index + 1, index + 2);
                    addIndices(index + 2, index + 1, index + 3);

                    index += 4;
                }
            }
        }                
        buildInterleavedVertices();
    }
}