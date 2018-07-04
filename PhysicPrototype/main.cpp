#include "olcConsoleGameEngineGLOOP.h"

using namespace std;

class PhysicsPrototype : public olcConsoleGameEngineGLOOP
{

public:
    PhysicsPrototype() {
        m_sAppName = L"PhysicsProtype";
    }

private:

public:
    virtual bool OnUserCreate() override
    {
        return true;
    };

    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        DrawString(100, 50, L"Hello World!");
        return true;
    };

};

int main(int argc, char** argv)
{
    PhysicsPrototype app;
    app.ConstructConsole(160, 100, 8, 8);
    app.Start();
    return 0;
}