#include "Rooms.h"

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

using namespace global;

#pragma region Pipes
Pipes::Pipes(Vec2 _position) : position(_position)
{    
    top_pipe = GameObject(Vec2{position.x, position.y - offset}, {position.x, position.y + offset, 533*SCALE, 2186*SCALE}, LoadTexture("assets/pipe.png"));
    bottom_pipe = GameObject(Vec2{position.x, position.y + offset}, {position.x, position.y - offset, 533*SCALE, 2186*SCALE}, LoadTexture("assets/pipe.png"));

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

Game::Game() : birb(Vec2{0, 0}, {0, 0, 0, 0}, LoadTexture("assets/birb.png")){}

#pragma region Game - Setup
void Game::Setup()
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    BackgroundColor = {70, 110, 180, 255};
    SetRandomSeed(time(0));

    camera.offset = {global::ScreenWidth*0.5f, global::ScreenHeight*0.5f};
    camera.rotation = 0.0f;
    camera.target = {0.0f, 0.0f};
    camera.zoom = 1.0f;

    timer = 0.0f;
    jump = -10.0f;
    gravity = 0.5f;
    score = 0;

    birb = GameObject(Vec2{-200, -90}, Rectangle{-200, -800, 50, 50}, LoadTexture("assets/birb.png"));
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
    BeginMode2D(camera);
    switch (state)
    {
    case State::Unpaused:
        {
            if (IsKeyPressed(KEY_ESCAPE)) state = State::Paused;
            ClearBackground(BackgroundColor);
            birb.velocity.y += gravity;
            if(IsKeyPressed(KEY_SPACE)) birb.velocity.y = jump;

            birb.Update(-1.0f, 6.0f);
            
            for(int i = pipes.size() - 1; i >= 0; i--)
            {
                pipes[i].Update();
                pipes[i].Draw(SCALE);
                if (collide(birb, pipes[i].top_pipe) || collide(birb, pipes[i].bottom_pipe))
                {
                    state = State::GameOver;
                    birb.velocity = {0.0f,  jump};
                }
                if (pipes[i].position.x + pipes[i].top_pipe.sprite.width*SCALE*0.5f + 2 < birb.position.x)
                {
                    if (!pipes[i].beenPassed)
                    {
                        pipes[i].beenPassed = true;
                        score++;
                    }
                    if (pipes[i].position.x < -430) pipes.erase(pipes.begin() + i);
                }
            }

            if(birb.position.y * sign(birb.position.y) > ScreenHeight*0.5f + 30)
            {
                state = State::GameOver;
                birb.velocity = {0.0f, jump};
            }

            birb.Draw(SCALE);
    
            EndMode2D();
            DrawText(TextFormat("%d\n", score), 450, 10, 21, BLACK);
            BeginMode2D(camera);
            timer += 1;

            if(timer >= 50) 
            {
                pipes.push_back(Pipes(Vec2{ 800.0f, (float)GetRandomValue(-132, 132) }));
                pipes[pipes.size() - 1].velocity = {-8, 0};
                timer = 0;
            }

            break;
        }

    case State::Paused:
        if (IsKeyPressed(KEY_ESCAPE)) state = State::Unpaused;
        ClearBackground(BackgroundColor);
        for(Pipes& p : pipes) p.Draw(SCALE);
        birb.Draw(SCALE);
        EndMode2D();

        DrawText(TextFormat("%d\n", score), 450, 10, 21, BLACK);

        if (GuiButton({ScreenWidth * 0.5f - 120, ScreenHeight * 0.5f - 30 - 50, 240, 60}, "Resume"))
        {
            state = State::Unpaused;
        }
        if (GuiButton({ScreenWidth * 0.5f - 120, ScreenHeight * 0.5f - 30 + 50, 240, 60}, "Return to main menu"))
            switchRooms = true;
            BeginMode2D(camera);
        break;

    case State::GameOver:
        if (IsKeyPressed(KEY_ESCAPE)) manager.current->Setup();
        ClearBackground(BackgroundColor);
        for(Pipes& p : pipes) p.Draw(SCALE);
        birb.Draw(SCALE, 0.0f, false, true);
        birb.Update();
        birb.velocity.y += gravity;
        EndMode2D();

        if(GuiButton({ScreenWidth  * 0.5f - 120, ScreenHeight * 0.5f - 30 - 50, 240, 60}, "Restart")) manager.current->Setup();
        if (GuiButton({ScreenWidth * 0.5f - 120, ScreenHeight * 0.5f - 30 + 50, 240, 60}, "Return to main menu"))
        {
            switchRooms = true;
        }
        BeginMode2D(camera);
        break;
    }

    EndMode2D();
    EndDrawing();

    if (switchRooms)
    {
        state = State::Unpaused;
        manager.SwitchTo<MainMenu>();
    }
}
#pragma endregion