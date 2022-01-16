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

		Plane(Transform* transform, RigidBody* rigidBody, Material* material, float width=50.0f,float height=50.0f) : Object(transform, rigidBody, material)
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


