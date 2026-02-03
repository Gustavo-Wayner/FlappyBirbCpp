#include "Rooms.h"

#include <raylib.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <format>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

using json = nlohmann::json;

namespace Assets
{
    Texture2D pipeTex;

    void LoadTextures()
    {
        pipeTex = LoadTexture("assets/pipe.png");
    }

    void UnloadTextures()
    {
        UnloadTexture(pipeTex);
    }
}

#pragma region Pipes
Pipes::Pipes(Vec2 _position) : position(_position)
{    
    top_pipe = GameObject(Vec2{position.x, position.y - offset}, {position.x, position.y + offset, 533*global::SCALE, 2186*global::SCALE}, Assets::pipeTex);
    bottom_pipe = GameObject(Vec2{position.x, position.y + offset}, {position.x, position.y - offset, 533*global::SCALE, 2186*global::SCALE}, Assets::pipeTex);

    velocity = {0, 0};
}

void Pipes::Update()
{
    position += velocity;
    bottom_pipe.velocity = velocity;
    top_pipe.velocity = velocity;

    bottom_pipe.Update();
    top_pipe.Update();
}

void Pipes::Draw(float scale)
{
    top_pipe.Draw(scale);
    bottom_pipe.Draw(scale, 0.0f, false, true);
}

void Pipes::DrawOrigin()
{
    DrawCircle(position.x, position.y, 3.0, BLACK);
    top_pipe.DrawOrigin(3.0);
    bottom_pipe.DrawOrigin(3.0);
}

void Pipes::DrawHbs()
{
    top_pipe.DrawHb();
    bottom_pipe.DrawHb();
}

#pragma endregion

#pragma region RoomManager

bool collide(const GameObject &a, const GameObject &b)
{
    Rectangle aHitbox = a.hitbox;
    Rectangle bHitbox = b.hitbox;

    return CheckCollisionRecs(aHitbox, bHitbox);
}


void RoomManager::Update()
{
    if (current)
        current->Step();
}
#pragma endregion

#pragma region MainMenu
void MainMenu::Setup()
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    std::ifstream file("assets/data.json");
    json j;
    file >> j;

    hs = j["highscore"];
}
void MainMenu::Step()
{
    bool over = false;
    bool switchRooms = false;
    BeginDrawing();
    ClearBackground(WHITE);
    if (global::showFPS) DrawFPS(20, 20);

    if (GuiCheckBox({ (float)global::ScreenWidth - 180, (float)global::ScreenHeight - 50, 20, 20 }, " Show fps", &global::showFPS))
    {
        std::ifstream file("assets/data.json");
        json j;
        file >> j;
        
        j["isShowFPSOn"] = global::showFPS;
        std::ofstream out("assets/data.json");

        out << j.dump(4);
        out.close();
    }

    if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 - 50, 240, 60}, "Play"))
        switchRooms = true;
    if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 + 50, 240, 60}, "Quit"))
    {
        over = true;
        global::closed = true;
    }

    DrawText(std::format("Hight score: {}", hs).c_str(), 10, global::ScreenHeight - 20, 15, BLACK);
    EndDrawing();
    if (switchRooms)
        global::manager.SwitchTo<Game>();
    if (over)
        CloseWindow();
}
#pragma endregion

Game::Game() : birb(Vec2{0, 0}, {0, 0, 0, 0}, LoadTexture("assets/birb.png")){}

#pragma region Game - Setup
void Game::Setup()
{
    Assets::LoadTextures();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    BackgroundColor = {70, 110, 180, 255};
    SetRandomSeed(time(0));

    camera.offset = {global::ScreenWidth*0.5f, global::ScreenHeight*0.5f};
    camera.rotation = 0.0f;
    camera.target = {0.0f, 0.0f};
    camera.zoom = 1.0f;

    timer = 0.0f;
    jump = -680.0f;
    gravity = 17.8f;
    global::score = 0;

    birb = GameObject(Vec2{-320, -90}, Rectangle{-200, -800, 50, 50}, LoadTexture("assets/birb.png"));
    birb.velocity = {0, 0};

    pipes.clear();
    state = State::Unpaused;
}
#pragma endregion

#pragma region Game - Step
void Game::Step()
{
    bool switchRooms = false;
    BeginDrawing();

    switch (state)
    {
    case State::Unpaused:
        {
            BeginMode2D(camera);
            if (IsKeyPressed(KEY_ESCAPE)) state = State::Paused;
            ClearBackground(BackgroundColor);
            birb.velocity.y += gravity * GetFrameTime();
            if(IsKeyPressed(KEY_SPACE)) birb.velocity.y = jump * GetFrameTime();

            birb.Update(-1.0f, 6.0f);
            
            for(int i = pipes.size() - 1; i >= 0; i--)
            {
                pipes[i].Update();
                pipes[i].Draw(global::SCALE);
                if (collide(birb, pipes[i].top_pipe) || collide(birb, pipes[i].bottom_pipe))
                {
                    state = State::GameOver;
                    birb.velocity = {0.0f,  jump * GetFrameTime()};
                }
                if (pipes[i].position.x + pipes[i].top_pipe.sprite.width*global::SCALE*0.5f + 2 < birb.position.x)
                {
                    if (!pipes[i].beenPassed)
                    {
                        pipes[i].beenPassed = true;
                        global::score++;
                    }
                    if (pipes[i].position.x < -470) pipes.erase(pipes.begin() + i);
                }
            }

            if(birb.position.y * sign(birb.position.y) > global::ScreenHeight*0.5f + 30)
            {
                state = State::GameOver;
                birb.velocity = {0.0f, jump * GetFrameTime()};
            }

            birb.Draw(global::SCALE);
    
            EndMode2D();
            DrawText(TextFormat("%d\n", global::score), 450, 10, 21, BLACK);
            if (global::showFPS) DrawFPS(20, 20);
            timer += 1;

            if(timer >= 95) 
            {
                pipes.push_back(Pipes(Vec2{ 800.0f, (float)GetRandomValue(-132, 132) }));
                pipes[pipes.size() - 1].velocity = {-520.0f * GetFrameTime(), 0};
                timer = 0;
            }

            break;
        }

    case State::Paused:
        if (IsKeyPressed(KEY_ESCAPE)) state = State::Unpaused;
        ClearBackground(BackgroundColor);

        BeginMode2D(camera);
        for(Pipes& p : pipes) p.Draw(global::SCALE);
        birb.Draw(global::SCALE);
        EndMode2D();

        if (global::showFPS) DrawFPS(20, 20);
        DrawText(TextFormat("%d\n", global::score), 450, 10, 21, BLACK);

        if (GuiButton({global::ScreenWidth * 0.5f - 120, global::ScreenHeight * 0.5f - 90, 240, 60}, "Resume"))
        {
            state = State::Unpaused;
        }
        if (GuiButton({global::ScreenWidth * 0.5f - 120, global::ScreenHeight * 0.5f + 30, 240, 60}, "Return to main menu"))
            switchRooms = true;
        break;

    case State::GameOver:
        std::ifstream file ("assets/data.json");
        
        json j;
        file >> j;

        if(global::score > j["highscore"])
        {
            j["highscore"] = global::score;
        
            std::ofstream out("assets/data.json");
            out << j.dump(4);
            out.close();
        }

        if (IsKeyPressed(KEY_ESCAPE)) global::manager.current->Setup();
        ClearBackground(BackgroundColor);

        BeginMode2D(camera);
        for(Pipes& p : pipes) p.Draw(global::SCALE);
        birb.Draw(global::SCALE, 0.0f, false, true);

        EndMode2D();
        if (global::showFPS) DrawFPS(20, 20);
        birb.Update();
        birb.velocity.y += gravity * GetFrameTime();

        if(GuiButton({global::ScreenWidth * 0.5f - 120, global::ScreenHeight * 0.5f - 90, 240, 60}, "Restart")) global::manager.current->Setup();
        if (GuiButton({global::ScreenWidth * 0.5f - 120, global::ScreenHeight * 0.5f + 30, 240, 60}, "Return to main menu"))
        {
            switchRooms = true;
        }
        break;
    }

    EndMode2D();
    EndDrawing();

    if (switchRooms)
    {
        std::ifstream file("assets/data.json");
        json j;
        file >> j;

        if(global::score > j["highscore"])
        {
            j["highscore"] = global::score;
        
            std::ofstream out("assets/data.json");
            out << j.dump(4);
            out.close();
        }

        Assets::UnloadTextures();
        state = State::Unpaused;
        global::manager.SwitchTo<MainMenu>();
    }
}
#pragma endregion