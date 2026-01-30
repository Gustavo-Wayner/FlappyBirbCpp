#include <raylib.h>
#include <iostream>
#include <ctime>

#include "Objects.h"
#include "Rooms.h"

int main()
{
    InitWindow(global::ScreenWidth, global::ScreenHeight, "Flappy Birb");
    SetTargetFPS(global::FPS);
    
    {
        Image icon = LoadImage("assets/birb.png");
        SetWindowIcon(icon);
        UnloadImage(icon);
    }
    
    global::manager.SwitchTo<MainMenu>();
    SetExitKey(KEY_NULL);
    
    while (!WindowShouldClose())
    {
        global::manager.Update();
    }

    if (!global::closed)
        CloseWindow();
    return 0;
}