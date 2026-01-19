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
    Vec2 velocity;
    Texture2D sprite;

    GameObject(Vec2 _position, Rectangle _hitbox, Texture2D _sprite);

    GameObject(const GameObject &other);

    void UpdateHitbox(const float& offsetX = 0.0f, const float& offsetY = 0.0f);
    Vec2 getPosition();
    GameObject getNext();

    GameObject &operator=(const GameObject &other);

    void Draw(float scale = 1, float rotation = 0)
    {
        DrawTextureEx(sprite, {position.x - sprite.width*0.5f*global::SCALE, position.y - sprite.height*0.5f*global::SCALE}, rotation, scale, WHITE);
    }

    void DrawOrigin(double size = 3.0)
    {
        DrawCircle(position.x, position.y, size, BLACK);
    }
    void DrawHb()
    {
        DrawRectangleLines(hitbox.x, hitbox.y, hitbox.width, hitbox.height, BLACK);
    }

    void Update(const float& offsetX = 0.0f, const float& offsetY = 0.0f)
    {
        UpdateHitbox(offsetX, offsetY);
        position += velocity;
    }
};