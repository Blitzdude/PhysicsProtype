#include "olcConsoleGameEngineGLOOP.h"
#include "Circle.h"
#include "QuadTree.h"

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
        qTree = new QuadTree({ ScreenWidth() / 2, ScreenHeight() / 2, ScreenWidth() / 2, ScreenHeight() / 2 }, 4);
		
		// Add random points to Circles vector
		for (int i = 0; i < 1000; i++)
		{
			int x = rand() % ScreenWidth();
			int y = rand() % ScreenHeight();
			circles.push_back(new Circle(x, y, 3, i));
		}

		/*  Go through the now populated vector
			and insert points into quadtree */
		for (auto itr : circles)
			qTree->Insert(new Point({ itr->x, itr->y, itr }));


        return true;
    };

    virtual bool OnUserUpdate(float fElapsedTime) override
    {
       
        // DRAWING //////////////////////////////////////

		// clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');
        // Get points in treenTree, pointsInArea;
        
		// move circles in a random direction
		const int maxMove = 2;
		for (auto itr : circles)
		{
			itr->x += ((rand() % maxMove * 2 + 1) - maxMove); // random number between -maxMove and +maxmove
			itr->y += ((rand() % maxMove * 2 + 1) - maxMove) ;
		}

		// TODO: radial checking here for overlap;
        // Go through the vector 
		std::vector<Circle*> overlappingCircles;
		for (auto itr : circles)
		{
			// For each element, check overlap from quadtree
			for (auto other : circles)
			{
				// if iterators aren't the same and distance is greater then r1+r2, then circles are overlapping

				if ( other->id != itr->id &&
					fabs((itr->x - other->x)*(itr->x - other->x) + (itr->y - other->y)*(itr->y - other->y)
						<= (itr->r + other->r)*(itr->r + other->r)) )
				{
					// circles are overlapping
					overlappingCircles.push_back(itr);
				}
				else 
				{
					// circles are not overlapping
				}
			}
		}

		// draw circles normally
		for (auto c : circles) FillCircle(c->x, c->y, c->r, PIXEL_HALF, BG_WHITE);
		// draw overlapping circles
		for (auto o : overlappingCircles) FillCircle(o->x, o->y, o->r, PIXEL_HALF, BG_GREEN);

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
            DrawQuadTree(qt->topLeft);
            DrawQuadTree(qt->topRight);
            DrawQuadTree(qt->bottomRight);
            DrawQuadTree(qt->bottomLeft);
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