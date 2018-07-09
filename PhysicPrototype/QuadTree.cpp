#include "QuadTree.h"

QuadTree::QuadTree(QuadRect b, int n) 
    : boundary(b), capacity(n)
{
}

QuadTree::~QuadTree()
{
}

bool QuadTree::Insert(Point * point)
{
    // AABB check if in bounds
    if (!Contains(point))
    {
        // point is not within boundaries
        return false;
    }

    if (points.size() < capacity)
    {
        points.push_back(point);
        return true;
    }
    else if (leaf)
    {
        Subdivide();
    }
    
    if (topLeft->Insert(point))
        return true;
    if (topRight->Insert(point))
        return true;
    if (bottomRight->Insert(point))
        return true;
    if (bottomLeft->Insert(point))
        return true;
    
    // Something went horribly wrong if it gets here.
    return false;
}

/* Divides the quadtree into four branches */
void QuadTree::Subdivide()
{
    QuadRect* tl = new QuadRect({boundary.cx - boundary.w_half / 2, boundary.cy - boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2  });
    topLeft = new QuadTree(*tl, capacity);
    QuadRect* tr = new QuadRect({ boundary.cx + boundary.w_half / 2, boundary.cy - boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    topRight = new QuadTree(*tr, capacity);
    QuadRect* br = new QuadRect({ boundary.cx + boundary.w_half / 2, boundary.cy + boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    bottomRight = new QuadTree(*br, capacity);
    QuadRect* bl = new QuadRect({ boundary.cx - boundary.w_half / 2, boundary.cy + boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    bottomLeft = new QuadTree(*bl, capacity);

    // this tree has been divided and is now a branch
    leaf = false;

}

bool QuadTree::Contains(Point * p)
{
    // AABB Check
    return (boundary.cx - boundary.w_half <= p->x &&
            boundary.cx + boundary.w_half >= p->x && 
            boundary.cy - boundary.h_half <= p->y &&
            boundary.cy + boundary.h_half >= p->y);
}

void QuadTree::getPoints(std::vector<Point*>& vec)
{
    // get the points in this node
    for (auto p : points)
        vec.push_back(p);

    // is this a branch?
    if (!leaf)
    {
        // getPoints on children 
        topLeft->getPoints(vec)    ;
        topRight->getPoints(vec)   ;
        bottomLeft->getPoints(vec) ;
        bottomRight->getPoints(vec);
    }
   
}

void QuadTree::getPointsInArea(std::vector<Point*>& vec, QuadRect area)
{
	if (!this->boundary.Intersects(area))
	{
		// returns empty if no points found
		return;
	}
	else
	{
		for (auto itr : points)
		{
			vec.push_back(itr);
		}

		if (!isLeaf())
		{
			topLeft->getPointsInArea(vec, area);
			topRight->getPointsInArea(vec, area);
			bottomRight->getPointsInArea(vec, area);
			bottomLeft->getPointsInArea(vec, area);
		}
	}

}
