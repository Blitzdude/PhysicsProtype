#pragma once
#include <vector>

struct Point{
    int x;
    int y;
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
};

class QuadTree
{
public:
    QuadTree(QuadRect b, int n );
    ~QuadTree();


    bool Insert(Point* point);
    void getPoints(std::vector<Point*>& vec);
	void getPointsInArea(std::vector<Point*>& vec, QuadRect area);

    // Getters
    bool isLeaf() { return leaf; };
    
private:

    void Subdivide();
    bool Contains(Point* p);

public:
    QuadRect boundary;

    QuadTree* topLeft     = nullptr;
    QuadTree* topRight    = nullptr;
    QuadTree* bottomRight = nullptr;
    QuadTree* bottomLeft  = nullptr;
private:

    int capacity;
    bool leaf = true;


    std::vector<Point*> points;

};

