#include <raylib.h>
#include <ctime>

#include "Objects.h"
#include "Rooms.h"

int main()
{
    SetWindowState(FLAG_VSYNC_HINT);
    InitWindow(900, 600, "Pong");
    SetTargetFPS(60);

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