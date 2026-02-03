#pragma once
#include <raylib.h>
#include <fstream>
#include <nlohmann/json.hpp>

struct RoomManager;

namespace global
{
    extern RoomManager manager;
    extern float SCALE;
    extern int ScreenWidth;
    extern int ScreenHeight;
    extern bool closed;
    extern bool showFPS;
    extern int FPS;
    extern int score;
}