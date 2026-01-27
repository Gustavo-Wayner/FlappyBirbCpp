#include <raylib.h>
#include <iostream>
#include <ctime>

#include "Objects.h"
#include "Rooms.h"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(global::ScreenWidth, global::ScreenHeight, "Flappy Birb");
    SetTargetFPS(60);

    global::manager.SwitchTo<MainMenu>();
    SetExitKey(KEY_NULL);

    while (!WindowShouldClose())
    {
        global::manager.Update();
        std::cout << global::vsyncEnabled << std::endl;
    }

    if (!global::closed)
        CloseWindow();
    return 0;
}