#include "QuadTree.h"

QuadTree::QuadTree(QuadRect b, int n)
	: boundary(b), capacity(n), depth(0)
{
}

QuadTree::QuadTree(QuadRect b, int n, int d)
    : boundary(b), capacity(n), depth(d)
{
}

QuadTree::~QuadTree()
{
	this->DestroyNode();
}

bool QuadTree::Insert(Point * point)
{
    // AABB check if in bounds
    if (!boundary.Contains(point))
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
		// insert currently held points to children
		for (auto p : points)
		{
			if (topLeft->Insert(p))
				continue;
			if (topRight->Insert(p))
				continue;
			if (bottomRight->Insert(p))
				continue;
			if (bottomLeft->Insert(p))
				continue;
		}
		// then clear the points
		points.clear();
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
    QuadRect tl = QuadRect({boundary.cx - boundary.w_half / 2, boundary.cy - boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2  });
	topLeft = std::make_unique<QuadTree>(tl, capacity, depth+1);
    QuadRect tr = QuadRect({ boundary.cx + boundary.w_half / 2, boundary.cy - boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    topRight = std::make_unique<QuadTree>(tr, capacity);
    QuadRect br = QuadRect({ boundary.cx + boundary.w_half / 2, boundary.cy + boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    bottomRight = std::make_unique<QuadTree>(br, capacity);
    QuadRect bl = QuadRect({ boundary.cx - boundary.w_half / 2, boundary.cy + boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    bottomLeft = std::make_unique<QuadTree>(bl, capacity);

	
    // this tree has been divided and is now a branch
    leaf = false;
}

void QuadTree::Query(std::vector<Point*>& vec)
{
	// get the points in this node
    for (auto p : points)
        vec.push_back(p);

    // is this a branch?
    if (!leaf)
    {
        // getPoints on children 
        topLeft->Query(vec);
        topRight->Query(vec);
        bottomRight->Query(vec);
		bottomLeft->Query(vec);
    }
   
	return;
}

std::vector<Point*> QuadTree::QueryArea(QuadRect area)
{
	std::vector<Point*> found;
	QueryAreaHelper(found, area);

	return found;
}

void QuadTree::QueryAreaHelper(std::vector<Point*>& vec, QuadRect area)
{
	// does the area intersect the boundary?
	if (!this->boundary.Intersects(area))
	{
		// returns empty if no points found
		return;
	}
	else
	{
		// are any of the points contained within the area?
		for (auto itr : points)
		{
			if (area.Contains(itr))
				vec.push_back(itr);
		}

		if (!isLeaf())
		{
			// ask the children for points
			topLeft->QueryAreaHelper(vec, area);
			topRight->QueryAreaHelper(vec, area);
			bottomRight->QueryAreaHelper(vec, area);
			bottomLeft->QueryAreaHelper(vec, area);
		}
	}
	
	return;
}

void QuadTree::DestroyNode()
{
	this->points.clear();
	this->boundary;
}

void QuadTree::ClearTree()
{
	if (!points.empty())
		points.clear();

	if (!isLeaf())
	{
		if (topLeft != nullptr)
		{
			topLeft->ClearTree();
			topLeft == nullptr;
		}
		if (topRight != nullptr)
		{
			topRight->ClearTree();
			topRight == nullptr;
		}
		if (bottomRight != nullptr)
		{
			bottomRight->ClearTree();
			bottomRight == nullptr;
		}
		if (bottomLeft != nullptr)
		{
			bottomLeft->ClearTree();
			bottomLeft == nullptr;
		}
		// all children cleared
		leaf = true;
	}

}
