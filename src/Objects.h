#pragma once

#include <raylib.h>
#include "Static.h"
#include "Structs.h"

class GameObject
{
public:
    Vector2 origin;
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

    void Draw(float scale = 1.0f, float rotation = 0.0f, bool flipX = false, bool flipY = false)
    {
        Rectangle src = {
            flipX ? (float)sprite.width : 0.0f,
            flipY ? (float)sprite.height : 0.0f,
            (float)sprite.width  * (flipX ? -1 : 1),
            (float)sprite.height * (flipY ? -1 : 1)
        };

        Rectangle dst = {
            position.x,
            position.y,
            sprite.width * scale,
            sprite.height * scale
        };

        DrawTexturePro(
            sprite,
            src,
            dst,
            { origin.x * scale, origin.y * scale },
            rotation,
            WHITE
        );
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