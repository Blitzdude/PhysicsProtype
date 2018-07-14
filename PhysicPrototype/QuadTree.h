#pragma once
#include "Circle.h"
#include <vector>
#include <memory>

struct Point{
	// Constructor
	Point(int _x, int _y, Circle* _userData)
		: x(_x), y(_y), userData(_userData)
	{};

    int x;
    int y;
	// void* userData;  /* could be used in the future, but probably just simpler to hold a circle */
	Circle* userData;
};
/*
    Rectangle defined by center
    and half width
*/
struct QuadRect {
    
    int cx;
    int cy;
    int w_half;
    int h_half;

    int Width() { return w_half * 2; };
    int Height() { return h_half * 2; };

	bool Intersects(QuadRect a) // const ref?
	{
		// AABB Check
		return (cx - w_half <= a.cx + a.w_half &&
			cx + w_half >= a.cx - a.w_half &&
			cy - h_half <= a.cy + a.h_half &&
			cy + h_half >= a.cy - a.h_half);
	}
	bool Contains(Point* p)
	{

		// AABB Check
		return (cx - w_half <= p->x &&
			cx + w_half >= p->x &&
			cy - h_half <= p->y &&
			cy + h_half >= p->y);
	}
};

class QuadTree
{
public:
    QuadTree(QuadRect b, int n );
    ~QuadTree();


    bool Insert(Point* point);
    void Query(std::vector<Point*>& vec);
	std::vector<Point*> QueryArea(QuadRect area);

	void ClearTree();
    // Getters
    bool isLeaf() { return leaf; };
    
private:

	void QueryAreaHelper(std::vector<Point*>& vec, QuadRect area);
    void Subdivide();

public:
    QuadRect boundary;

    std::unique_ptr<QuadTree> topLeft     = nullptr;
    std::unique_ptr<QuadTree> topRight    = nullptr;
    std::unique_ptr<QuadTree> bottomRight = nullptr;
    std::unique_ptr<QuadTree> bottomLeft  = nullptr;
private:

    unsigned int capacity;
    bool leaf = true;


    std::vector<Point*> points;

};

