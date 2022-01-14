#ifndef SIMPLEPHYSICSENGINE_SPHERE_H
#include "Object.h"
#include "PhysicsEngine.h"

namespace SimplePhysicsEngine
{
	/// <summary>
	/// Object that draws on XZ Plane.	
	/// </summary>
	class Plane : public Object
	{
	public:
		float width;
		float height;

		Plane(utils::Vector3 position = utils::Vector3{ 0,0,0 }, utils::Vector3 rotation = utils::Vector3{ 0,0,0 }, utils::Vector3 velocity = utils::Vector3{ 0,0,0 }, utils::Vector3 forces = utils::Vector3{ 0,0,0 }, float mass = 5.0f, bool usePhysics = true, float width=50.0f,float height=50.0f, Shader* shader = 0) : Object(position, rotation, velocity, forces, mass, usePhysics, shader)
		{		
			this->width = width;
			this->height = height;
			buildVertices();
		}
	private:
		void buildVertices();
	};
}

#endif // !SPHERE_H


