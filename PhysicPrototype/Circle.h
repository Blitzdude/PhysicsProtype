#pragma once

struct QuadRect;

class Circle
{
public:
	Circle(int _x, int _y, int _r, unsigned int _id);
	~Circle();

	QuadRect AABB();
public:
	int x;
	int y;
	int r;
	unsigned int id;
};
