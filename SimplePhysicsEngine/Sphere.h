#pragma once
#ifndef SIMPLEPHYSICSENGINE_SPHERE_H
#define SIMPLEPHYSICSENGINE_SPHERE_H
#include "Object.h"
#include "PhysicsEngine.h"

namespace SimplePhysicsEngine
{
	class Sphere : public Object
	{
	public:
		float radius;
		int sectorCount;
		int stackCount;

		Sphere(Transform* transform, RigidBody* rigidBody, Material* material, float radius = 1.0f, int sectorCount = 36, int stackCount = 18) : Object(transform, rigidBody, material)
		{			
			this->radius = radius;
			this->sectorCount = sectorCount;
			this->stackCount = stackCount;
			BuildVertices();
		}
	private:
		void BuildVertices();
	};
}

#endif // !SPHERE_H


