#pragma once
#include<array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Simplex
{
private:
	std::array<glm::vec3, 4> points;
	unsigned dim;
public:
	Simplex() : points({ glm::vec3(),glm::vec3(),glm::vec3(),glm::vec3() }), dim(0) {}

	Simplex& operator=(std::initializer_list<glm::vec3> list);
	glm::vec3& operator[](unsigned i) { return points[i]; }
	void Push(glm::vec3 pnt);
	unsigned Size() const { return dim; };
	auto Begin() const { return points.begin(); }
	auto End() const { return points.end() - (4 - dim); }
};

