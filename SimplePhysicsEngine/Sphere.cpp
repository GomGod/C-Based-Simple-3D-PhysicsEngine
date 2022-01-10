#include "Sphere.h"

void Sphere::buildVertices()
{
    const float PI = acos(-1);

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);       // r * cos(u)
        float z = radius * sinf(stackAngle);        // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
            vertex.z = z;                           // z = r * sin(u)
            vertex.s = (float)j / sectorCount;        // s
            vertex.t = (float)i / stackCount;         // t
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
    std::vector<float> n;                           // 1 face normal

    int i, j, k, vi1, vi2;
    int index = 0;                                  // index for vertex
    for (i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1);                // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if (i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);

                // put normal
                n = getFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);


                index += 3;     // for next
            }
            else if (i == (stackCount - 1)) // a triangle for last stack =========
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);

                // put normal
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);

                index += 3;     // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v4.x, v4.y, v4.z);


                // put normal
                n = getFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of quad (2 triangles)
                addIndices(index, index + 1, index + 2);
                addIndices(index + 2, index + 1, index + 3);

                index += 4;     // for next
            }
        }
    }

    buildInterleavedVertices();

	/*const float PI = acos(-1);

	struct Vertex { float x, y, z, s, t; };
	std::vector<Vertex> tmpVerticies;

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float stackAngle, sectorAngle;

	for (auto i = 0; i <= stackCount; i+=1)
	{
		stackAngle = PI / 2 - i * stackStep;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (auto j = 0; j <= sectorCount; j += 1)
		{
			sectorAngle = j * sectorStep;

			Vertex vertex;
			vertex.x = xy * cosf(sectorAngle);
			vertex.y = xy * sinf(sectorAngle);
			vertex.z = z;
			vertex.s = (float)j / sectorCount;
			vertex.t = (float)i / stackCount;

			tmpVerticies.push_back(vertex);
		}
	}

	clearArrays();

	Vertex v1, v2, v3, v4;
	std::vector<float> n;

	int i, j, k, vi1, vi2;
	int index = 0;
	
	//  v1--v3
	//  |    |
	//  v2--v4
	for (i = 0; i < stackCount; i += 1)
	{
		vi1 = i * (sectorCount + 1);
		vi2 = (i + 1) * (sectorCount + 1);
		v1 = tmpVerticies[vi1];
		v2 = tmpVerticies[vi2];
		v3 = tmpVerticies[vi1 + 1];
		v4 = tmpVerticies[vi2 + 1];

		if (i == 0)
		{
			addVertex(v1.x, v1.y, v1.z);
			addVertex(v2.x, v2.y, v2.z);
			addVertex(v4.x, v4.y, v4.z);

			n = getFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);

			for (k = 0; k < 3; k += 1)
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

			n = getFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
			for (k = 0; k < 3; k += 1)
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
			for (k = 0; k < 4; ++k)  // same normals for 4 vertices
			{
				addNormal(n[0], n[1], n[2]);
			}

			// put indices of quad (2 triangles)
			addIndices(index, index + 1, index + 2);
			addIndices(index + 2, index + 1, index + 3);

			index += 4;     // for next
		}	
	}*/
}

Sphere::Sphere(float radius, int sectorCount, int stackCount) : radius(radius), sectorCount(sectorCount), stackCount(stackCount)
{
	buildVertices();
}
