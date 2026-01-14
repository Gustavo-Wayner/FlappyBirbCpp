#pragma once

#include <ctime>
#include <cstdlib>
#include <vector>

#include "Objects.h"
#include "Static.h"

bool collide(const GameObject &a, const GameObject &b);

class Pipes
{
public:
    int offset = 220;
    Vec2 position;
    Vec2 velocity;
    GameObject top_pipe;
    GameObject bottom_pipe;

    Pipes(Vec2 _position);
    void Draw(float scale = 1);
    void DrawOrigin();
    void Update();
};

enum State
{
    Unpaused,
    Paused
};

struct Room
{
    Camera2D camera = {0};
    bool pressedPause = false;
    State state = State::Unpaused;
    Color BackgroundColor;
    virtual void Setup() {}
    virtual void Step() {}
};

struct RoomManager
{
    Room *current = nullptr;

    template <typename T>
    void SwitchTo()
    {
        static T roomInstance;
        roomInstance.Setup();
        current = &roomInstance;
    }

    void Update();
};

struct MainMenu : public Room
{
    void Setup() override;
    void Step() override;
};

struct Game : public Room
{
    float scale;
    int score;
    GameObject birb;
    float gravity;
    float jump;
    std::vector<Pipes> pipes;

    Game();

    void Setup() override;
    void Step() override;
};