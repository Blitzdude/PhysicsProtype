#include "olcConsoleGameEngineGLOOP.h"
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
    std::vector<QuadRect*> rectangles;

public:
    virtual bool OnUserCreate() override
    {
        qTree = new QuadTree({ ScreenWidth() / 2, ScreenHeight() / 2, ScreenWidth() / 2, ScreenHeight() / 2 }, 4);


        return true;
    };

    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        if (m_mouse[0].bPressed == true)
        {
            Point* mouseCoord = new Point({m_mousePosX, m_mousePosY});
            qTree->Insert(mouseCoord);
        }

        // Drawing 
        // Get points in tree
        std::vector<Point*> pointsInTree;
        qTree->getPoints(pointsInTree);
        
        // draw Tree areas
        DrawQuadTree(qTree);
        
        // draw points
        for (auto p : pointsInTree)
            Draw(p->x, p->y);
       
        
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