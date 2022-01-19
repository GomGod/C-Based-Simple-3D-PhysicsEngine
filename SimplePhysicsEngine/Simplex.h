#pragma once
#include<array>
#include"Vector3.h"

class Simplex
{
private:
	std::array<utils::Vector3, 4> points;
	unsigned dim;
public:
	Simplex() : points({ utils::Vector3(),utils::Vector3(),utils::Vector3(),utils::Vector3() }), dim(0) {}

	Simplex& operator=(std::initializer_list<utils::Vector3> list);
	utils::Vector3& operator[](unsigned i) { return points[i]; }
	void push(utils::Vector3 pnt);
	unsigned size() const { return dim; };
	auto begin() const { return points.begin(); }
	auto end() const { return points.end() - (4 - dim); }
};

