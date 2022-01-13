#ifndef SIMPLEPHYSICSENGINE_SPHERE_H
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

		Sphere(utils::Vector3 position = utils::Vector3{ 0,0,0 }, utils::Vector3 rotation = utils::Vector3{ 0,0,0 }, utils::Vector3 velocity = utils::Vector3{ 0,0,0 }, utils::Vector3 forces = utils::Vector3{ 0,0,0 }, float mass = 5.0f, bool usePhysics = true, float radius = 1.0f, int sectorCount = 36, int stackCount = 18, Shader* shader = 0) : Object(position, rotation, velocity, forces, mass, usePhysics, shader)
		{
			this->radius = radius;
			this->sectorCount = sectorCount;
			this->stackCount = stackCount;
			buildVertices();
		}
	private:
		void buildVertices();
	};
}

#endif // !SPHERE_H


