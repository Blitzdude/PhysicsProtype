#include "Circle.h"
#include "QuadTree.h"
Circle::Circle(int _x, int _y, int _r, unsigned int _id)
	: x(_x), y(_y), r(_r), id(_id)
{
}

Circle::~Circle()
{
}


QuadRect Circle::AABB()
{
	return QuadRect({ x, y, r, r });
}
