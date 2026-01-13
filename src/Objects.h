#pragma once

#include <raylib.h>
#include "Structs.h"

class GameObject
{
public:
    Vec2 position = {0.0f, 0.0f};
    Rectangle hitbox = {0, 0, 0, 0};

    GameObject() = default;
    double width, height;
    Vec2 velocity;
    Texture2D sprite;

    GameObject(Vec2 _position, double _width, double _height, Texture2D _sprite);

    GameObject(const GameObject &other);

    void UpdateHitbox();
    Vec2 getPosition();
    GameObject getNext();

    GameObject &operator=(const GameObject &other);

    void Draw()
    {
        DrawTexture(sprite, position.x, position.y, WHITE);
    }

    Rectangle Hitbox() const
    {
        return hitbox;
    }

    void DrawOrigin()
    {
        DrawCircle(position.x, position.y, 3.0, BLACK);
    }

    void Update()
    {
        position += velocity;
        UpdateHitbox();
    }
};