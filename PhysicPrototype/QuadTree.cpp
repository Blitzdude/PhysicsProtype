#include "QuadTree.h"

QuadTree::QuadTree(QuadRect b, int n) 
    : boundary(b), capacity(n)
{
}

QuadTree::~QuadTree()
{
	//this->ClearTree();
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
	topLeft = std::make_unique<QuadTree>(*tl, capacity);
    QuadRect* tr = new QuadRect({ boundary.cx + boundary.w_half / 2, boundary.cy - boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    topRight = std::make_unique<QuadTree>(*tr, capacity);
    QuadRect* br = new QuadRect({ boundary.cx + boundary.w_half / 2, boundary.cy + boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    bottomRight = std::make_unique<QuadTree>(*br, capacity);
    QuadRect* bl = new QuadRect({ boundary.cx - boundary.w_half / 2, boundary.cy + boundary.h_half / 2, boundary.w_half / 2, boundary.h_half / 2 });
    bottomLeft = std::make_unique<QuadTree>(*bl, capacity);

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

void QuadTree::QueryArea(std::vector<Point*>& vec, QuadRect area)
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
			topLeft->QueryArea(vec, area);
			topRight->QueryArea(vec, area);
			bottomRight->QueryArea(vec, area);
			bottomLeft->QueryArea(vec, area);
		}
	}
	
	return;
}

void QuadTree::ClearTree()
{
	if (!this->isLeaf())
	{
		topLeft->ClearTree();
		topRight->ClearTree();
		bottomLeft->ClearTree();
		bottomRight->ClearTree();
		
		topLeft = nullptr;
		topRight = nullptr;
		bottomLeft = nullptr;
		bottomRight = nullptr;

		this->leaf = false;
	}

	points.clear();
}
