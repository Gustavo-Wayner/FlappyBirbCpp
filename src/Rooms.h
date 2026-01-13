#pragma once

#include <ctime>
#include <cstdlib>

#include "Objects.h"
#include "Static.h"

bool collide(const GameObject &a, const GameObject &b);

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
    int score;
    GameObject birb;
    float gravity;
    float jump;

    Game();

    void Setup() override;
    void Step() override;
};