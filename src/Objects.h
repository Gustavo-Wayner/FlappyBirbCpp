#pragma once

#include <raylib.h>
#include "Static.h"
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

    void Draw(float scale = 1, float rotation = 0)
    {
        DrawTextureEx(sprite, {position.x - sprite.width*0.5f*global::SCALE, position.y - sprite.height*0.5f*global::SCALE}, rotation, scale, WHITE);
    }

    Rectangle Hitbox() const
    {
        return hitbox;
    }

    void DrawOrigin(double size = 3.0)
    {
        DrawCircle(position.x, position.y, size, BLACK);
    }

    void Update()
    {
        position += velocity;
        UpdateHitbox();
    }
};