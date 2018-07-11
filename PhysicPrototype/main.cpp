#include "olcConsoleGameEngineGLOOP.h"
#include "QuadTree.h"

using namespace std;

struct Circle {
	Point pos;
	int r;
	
	QuadRect AABB() { return { pos.x, pos.y, r, r }; };
};

class PhysicsPrototype : public olcConsoleGameEngineGLOOP
{

public:
    PhysicsPrototype() {
        m_sAppName = L"PhysicsProtype";
    }

private:
    QuadTree* qTree = nullptr;
	std::vector<Circle*> circles;
	QuadRect searchArea;
public:
    virtual bool OnUserCreate() override
    {
		// lets not use quadtree yet
        //qTree = new QuadTree({ ScreenWidth() / 2, ScreenHeight() / 2, ScreenWidth() / 2, ScreenHeight() / 2 }, 4);
		
		// Add random points
		for (int i = 0; i < 100; i++)
		{
			int x = rand() % ScreenWidth();
			int y = rand() % ScreenHeight();
			circles.push_back(new Circle({ Point({ x,y }), 2 }));
			//qTree->Insert(new Point({ x, y }) );
		}

		searchArea = { 0, 0, 20, 20 };

        return true;
    };

    virtual bool OnUserUpdate(float fElapsedTime) override
    {
		/*
        if (m_mouse[0].bPressed == true)
        {
            Point* mouseCoord = new Point({m_mousePosX, m_mousePosY});
            qTree->Insert(mouseCoord);
        }
		*/

        // Drawing 
		// clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');
        // Get points in treenTree, pointsInArea;
        
        
        // draw points
		for (auto c : circles)
            FillCircle(c->pos.x, c->pos.y, c->r);
       
		searchArea.cx = m_mousePosX;
		searchArea.cy = m_mousePosY;

		for (auto c : circles)
		{
			// TODO: radial checking here for overlap;
		}
        
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