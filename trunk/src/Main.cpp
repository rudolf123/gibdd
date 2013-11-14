#include "Precompiled.h"
#include "Root.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    Gibdd::Root* root = new Gibdd::Root();
    if(!root->Initialize())
    {
        delete root;
        return 0;
    }
    root->StartEngine();

    delete root;
    return 0;
}