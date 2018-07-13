#include "olcConsoleGameEngineGLOOP.h"
#include "Circle.h"
#include "QuadTree.h"

#include <algorithm>

using namespace std;


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
        qTree = new QuadTree({ ScreenWidth() / 2, ScreenHeight() / 2, ScreenWidth() / 2, ScreenHeight() / 2 }, 4); // remade every draw call
		
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
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');
        
		// move circles in a random direction
		const int maxMove = 2;
		for (auto itr : circles)
		{
			itr->x += ((rand() % maxMove * 2 + 1) - maxMove); // random number between -maxMove and +maxmove
			itr->y += ((rand() % maxMove * 2 + 1) - maxMove) ;
		}

		// rebuild the quadtree with new circles
		

		// Go through the vector 
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

		// draw circles normally
		for (auto c : circles) FillCircle(c->x, c->y, c->r, PIXEL_HALF, BG_WHITE);
		// draw overlapping circles
		for (auto o : overlappingCircles) FillCircle(o->x, o->y, o->r, PIXEL_HALF, BG_GREEN);

		// clear the QuadTree, because all the points will have moved

        return true;
    };

    void DrawQuadTree(QuadTree* qt)
    {
        // draw boundary
        QuadRect b = qt->boundary;
        DrawRectangle(b.cx - b.w_half, b.cy - b.h_half, b.Width()-1, b.Height()-1, PIXEL_SOLID, FG_GREEN);
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