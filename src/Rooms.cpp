#include "Rooms.h"

#include <raylib.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <format>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

using namespace global;
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
    top_pipe = GameObject(Vec2{position.x, position.y - offset}, {position.x, position.y + offset, 533*SCALE, 2186*SCALE}, Assets::pipeTex);
    bottom_pipe = GameObject(Vec2{position.x, position.y + offset}, {position.x, position.y - offset, 533*SCALE, 2186*SCALE}, Assets::pipeTex);

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
    if (showFPS) DrawFPS(20, 20);

    if (GuiCheckBox({ (float)ScreenWidth - 180, (float)ScreenHeight - 50, 20, 20 }, " Show fps", &showFPS))
    {
        json j;
        file >> j;
        
        j["isShowFPSOn"] = showFPS;
    }

    if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 - 50, 240, 60}, "Play"))
        switchRooms = true;
    if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 + 50, 240, 60}, "Quit"))
    {
        over = true;
        closed = true;
    }

    DrawText(std::format("Hight score: {}", hs).c_str(), 10, ScreenHeight + 20, 15, BLACK);
    EndDrawing();
    if (switchRooms)
        manager.SwitchTo<Game>();
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
    score = 0;

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
                pipes[i].Draw(SCALE);
                if (collide(birb, pipes[i].top_pipe) || collide(birb, pipes[i].bottom_pipe))
                {
                    state = State::GameOver;
                    birb.velocity = {0.0f,  jump * GetFrameTime()};
                }
                if (pipes[i].position.x + pipes[i].top_pipe.sprite.width*SCALE*0.5f + 2 < birb.position.x)
                {
                    if (!pipes[i].beenPassed)
                    {
                        pipes[i].beenPassed = true;
                        score++;
                    }
                    if (pipes[i].position.x < -470) pipes.erase(pipes.begin() + i);
                }
            }

            if(birb.position.y * sign(birb.position.y) > ScreenHeight*0.5f + 30)
            {
                state = State::GameOver;
                birb.velocity = {0.0f, jump * GetFrameTime()};
            }

            birb.Draw(SCALE);
    
            EndMode2D();
            DrawText(TextFormat("%d\n", score), 450, 10, 21, BLACK);
            if (showFPS) DrawFPS(20, 20);
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
        for(Pipes& p : pipes) p.Draw(SCALE);
        birb.Draw(SCALE);
        EndMode2D();

        if (showFPS) DrawFPS(20, 20);
        DrawText(TextFormat("%d\n", score), 450, 10, 21, BLACK);

        if (GuiButton({ScreenWidth * 0.5f - 120, ScreenHeight * 0.5f - 90, 240, 60}, "Resume"))
        {
            state = State::Unpaused;
        }
        if (GuiButton({ScreenWidth * 0.5f - 120, ScreenHeight * 0.5f + 30, 240, 60}, "Return to main menu"))
            switchRooms = true;
        break;

    case State::GameOver:
        json j;
        file >> j;

        if(score > j["highscore"])
        {
            j["highscore"] = score;
        
            std::ofstream out("assets/data.json");
            out << j.dump(4);
        }

        if (IsKeyPressed(KEY_ESCAPE)) manager.current->Setup();
        ClearBackground(BackgroundColor);

        BeginMode2D(camera);
        for(Pipes& p : pipes) p.Draw(SCALE);
        birb.Draw(SCALE, 0.0f, false, true);

        EndMode2D();
        if (showFPS) DrawFPS(20, 20);
        birb.Update();
        birb.velocity.y += gravity * GetFrameTime();

        if(GuiButton({ScreenWidth * 0.5f - 120, ScreenHeight * 0.5f - 90, 240, 60}, "Restart")) manager.current->Setup();
        if (GuiButton({ScreenWidth * 0.5f - 120, ScreenHeight * 0.5f + 30, 240, 60}, "Return to main menu"))
        {
            switchRooms = true;
        }
        break;
    }

    EndMode2D();
    EndDrawing();

    if (switchRooms)
    {
        json j;
        file >> j;

        if(score > j["highscore"])
        {
            j["highscore"] = score;
        
            std::ofstream out("assets/data.json");
            out << j.dump(4);
        }

        Assets::UnloadTextures();
        state = State::Unpaused;
        manager.SwitchTo<MainMenu>();
    }
}
#pragma endregion