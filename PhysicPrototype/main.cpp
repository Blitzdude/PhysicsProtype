#include "olcConsoleGameEngineGLOOP.h"
#include "Circle.h"
#include "QuadTree.h"

#include <algorithm>

using namespace std;

//#define NOTREE
#define QUADTREE

class PhysicsPrototype : public olcConsoleGameEngineGLOOP
{

public:
    PhysicsPrototype() {
        m_sAppName = L"PhysicsProtype";
    }

private:
    QuadTree* qTree = nullptr;
	std::vector<Circle*> circles;

public:
    virtual bool OnUserCreate() override
    {
		// Create a new quadTree;
        qTree = new QuadTree({ ScreenWidth() / 2, ScreenHeight() / 2, ScreenWidth() / 2, ScreenHeight() / 2 }, 2); // remade every draw call
		
		// Add random points to Circles vector
		for (int i = 0; i < 100; i++)
		{
			int x = rand() % ScreenWidth();
			int y = rand() % ScreenHeight();
			circles.push_back(new Circle(x, y, 3, i));
		}
        return true;
    };

    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        // DRAWING //////////////////////////////////////

		// clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, BG_BLACK);
        
		// move circles in a random direction
		const int maxMove = 2;
		for (auto itr : circles)
		{
			itr->x += ((rand() % maxMove * 2 + 1) - maxMove); // random number between -maxMove and +maxmove
			itr->y += ((rand() % maxMove * 2 + 1) - maxMove) ;
		}

#ifdef QUADTREE
		// Clear the quadtree
		qTree->ClearTree();
		// rebuild the quadtree with new circles
		std::vector<Circle*> overlappingCircles;
		for (auto p : circles)
		{
			qTree->Insert(new Point{ p->x, p->y, p });
		}
		// for each circle in scene
		for (auto c : circles)
		{
			// search the area for possible overlap
			auto circlesToCheck = qTree->QueryArea(c->AABB());
			for (auto other : circlesToCheck)
			{
				if (c->id != other->userData->id &&
					sqrt(fabs((c->x - other->userData->x) + (c->y - other->userData->y)))
					<= c->r + other->userData->r)
				{
					// circle is overlapping
					overlappingCircles.push_back(c);
				}
			}
		}
#endif
		

#ifdef NOTREE
		// Go through all the circles
		std::vector<Circle*> overlappingCircles;
		for (auto itr : circles)
		{
			for (auto other : circles)
			{
				// if circle is not itself && they are overlapping
				if (itr->id != other->id &&
					fabs((itr->x - other->x)*(itr->x - other->x) + (itr->y - other->y)*(itr->y - other->y) )
					<= (itr->r + other->r)*(itr->r + other->r))
				{
					// circle is overlapping
					overlappingCircles.push_back(itr);
				}
			}
		}
#endif

		// draw circles normally
		for (auto c : circles) FillCircle(c->x, c->y, c->r, PIXEL_HALF, BG_WHITE);
		// draw overlapping circles
		for (auto o : overlappingCircles) FillCircle(o->x, o->y, o->r, PIXEL_HALF, BG_GREEN);

		// draw quadtree
		DrawQuadTree(qTree);
        return true;
    };

    void DrawQuadTree(QuadTree* qt)
    {
        // draw boundary
        QuadRect b = qt->boundary;
        DrawRectangle(b.cx - b.w_half, b.cy - b.h_half, b.Width(), b.Height(), PIXEL_SOLID, FG_GREEN);
        // check if is leaf
        if (!qt->isLeaf())
        {
            // call DrawQuadTree on children    
            DrawQuadTree(qt->topLeft.get());
            DrawQuadTree(qt->topRight.get());
            DrawQuadTree(qt->bottomRight.get());
            DrawQuadTree(qt->bottomLeft.get());
        }
    }
};

int main(int argc, char** argv)
{
    PhysicsPrototype app;
    app.ConstructConsole(360, 240, 4, 4);
    app.Start();
    return 0;
}