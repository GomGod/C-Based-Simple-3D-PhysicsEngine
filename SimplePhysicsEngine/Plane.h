#pragma once
#ifndef SIMPLEPHYSICSENGINE_PLANE_H
#define SIMPLEPHYSICSENGINE_PLANE_H
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
		float thickness;

		Plane(Transform* transform, RigidBody* rigidBody, Material* material, float width = 50.0f, float height = 50.0f, float thickness = 10.0f) : Object(transform, rigidBody, material)
		{
			this->thickness = thickness;
			this->width = width;
			this->height = height;
			BuildVertices();
		}
	private:
		void BuildVertices();
	};
}

#endif
