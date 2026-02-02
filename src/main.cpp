#include <raylib.h>
#include <filesystem>
#include <ctime>
#include <nlohmann/json.hpp>

#include "Objects.h"
#include "Rooms.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

int main()
{
    json j;
    if(!fs::exists("assets/data.json"))
    {
        j["isShowFPSOn"] = false;
        j["highscore"] = 0;
        
        std::ofstream out("assets/data.json");
        out << j.dump(4);
        out.close();
    }
    
    global::file = std::ifstream("assets/data.json");

    global::file >> j;
    global::showFPS = j["isShowFPSOn"];

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

    if(global::score > j["highscore"])
    {
        j["highscore"] = global::score;
    }
    
    std::ofstream out("assets/data.json");
    out << j.dump(4);
    if (!global::closed)
        CloseWindow();
    return 0;
}