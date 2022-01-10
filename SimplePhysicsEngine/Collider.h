#ifndef SIMPLEPHYSICSENGINE_COLLIDER_H
#define SIMPLEPHYSICSENGINE_COLLIDER_H

#include "Vector3.h"
#include <vector>

namespace components
{
	class Collider
	{
	private:
		std::vector<utils::Vector3> vertices;

	public:
		void SetVertices();
		virtual void CheckCollision(const Collider& col1, const Collider& col2);
	};
}
#endif