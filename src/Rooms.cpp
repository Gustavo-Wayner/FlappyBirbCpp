#include "Rooms.h"

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

using namespace global;

#pragma region RoomManager

bool collide(const GameObject &a, const GameObject &b)
{
    Rectangle aHitbox = a.Hitbox();
    Rectangle bHitbox = b.Hitbox();

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
}
void MainMenu::Step()
{
    bool over = false;
    bool switchRooms = false;
    BeginDrawing();
    ClearBackground(WHITE);
    if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 - 50, 240, 60}, "Play"))
        switchRooms = true;
    if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 + 50, 240, 60}, "Quit"))
    {
        over = true;
        closed = true;
    }
    EndDrawing();
    if (switchRooms)
        manager.SwitchTo<Game>();
    if (over)
        CloseWindow();
}
#pragma endregion

#pragma region Game

Game::Game() : birb(Vec2{0, 0}, 0, 0, LoadTexture("assets/birb.png")){}

void Game::Setup()
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    BackgroundColor = {70, 110, 180, 255};
    SetRandomSeed(time(0));

    camera.offset = {global::ScreenWidth*0.5f, global::ScreenHeight*0.5f};
    camera.rotation = 0.0f;
    camera.target = {0.0f, 0.0f};
    camera.zoom = 0.15f;

    jump = -40.0f;
    gravity = 2.0f;

    birb = GameObject(Vec2{-2000, -900}, 40, 40, LoadTexture("assets/birb.png"));
    birb.velocity = {0, 0};

    state = State::Unpaused;
}

void Game::Step()
{
    bool switchRooms = false;
    BeginDrawing();
    BeginMode2D(camera);
    switch (state)
    {
    case State::Unpaused:
        ClearBackground(BackgroundColor);
        birb.velocity.y += gravity;
        if(IsKeyPressed(KEY_SPACE)) birb.velocity.y = jump;

        birb.Update();
        birb.Draw();

        EndMode2D();
        DrawText(TextFormat("%d\n", score), 450, 10, 21, BLACK);
        BeginMode2D(camera);
        break;

    case State::Paused:
        ClearBackground(BackgroundColor);
        birb.Draw();
        EndMode2D();

        DrawText(TextFormat("%d\n", score), 450, 10, 21, BLACK);

        if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 - 50, 240, 60}, "Resume"))
        {
            state = State::Unpaused;
        }
        if (GuiButton({GetScreenWidth() * 0.5f - 120, GetScreenHeight() * 0.5f - 30 + 50, 240, 60}, "Return to main menu"))
            switchRooms = true;
            BeginMode2D(camera);
        break;
    }

    EndMode2D();
    EndDrawing();

    if (IsKeyDown(KEY_ESCAPE))
    {
        if (!pressedPause)
        {
            switch (state)
            {
            case State::Unpaused:
                state = State::Paused;
                break;
            case State::Paused:
                state = State::Unpaused;
                break;
            }
        }
        pressedPause = true;
    }
    else
    {
        pressedPause = false;
    }

    if (switchRooms)
    {
        state = State::Unpaused;
        manager.SwitchTo<MainMenu>();
    }
}
#pragma endregion