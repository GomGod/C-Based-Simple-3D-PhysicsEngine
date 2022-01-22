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
	void Push(utils::Vector3 pnt);
	unsigned Size() const { return dim; };
	auto Begin() const { return points.begin(); }
	auto End() const { return points.end() - (4 - dim); }
};

