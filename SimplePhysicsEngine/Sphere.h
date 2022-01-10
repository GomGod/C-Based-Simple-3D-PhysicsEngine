#ifndef SIMPLEPHYSICSENGINE_SPHERE_H
#include "Object.h"

class Sphere : public Object
{
public:
	float radius;
	int sectorCount;
	int stackCount;

	Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
	~Sphere() {}

private:
	void buildVertices();	
};


#endif // !SPHERE_H


