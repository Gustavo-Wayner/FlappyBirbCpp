#include "Objects.h"

GameObject::GameObject(Vec2 _position, Rectangle _hitbox, Texture2D _sprite) : position(_position), sprite(_sprite), hitbox(_hitbox), velocity({0, 0})
{
    origin = { sprite.width * 0.5f, sprite.height * 0.5f };
    UpdateHitbox();
}

GameObject::GameObject(const GameObject &other) : position(other.position), sprite(other.sprite), hitbox(other.hitbox), velocity(other.velocity)
{
    velocity = other.velocity;
    origin = { other.sprite.width * 0.5f, other.sprite.height * 0.5f };
    UpdateHitbox();
}

Vec2 GameObject::getPosition() { return position; }

void GameObject::UpdateHitbox(const float& offsetX, const float& offsetY)
{
    hitbox.x = position.x - hitbox.width * 0.5f + offsetX;
    hitbox.y = position.y - hitbox.height * 0.5f + offsetY;
}

GameObject &GameObject::operator=(const GameObject &other)
{
    position = other.position;
    origin = { other.sprite.width * 0.5f, other.sprite.height * 0.5f };

    velocity = other.velocity;
    sprite = other.sprite;

    hitbox.width = other.hitbox.width;
    hitbox.height = other.hitbox.height;
    hitbox.x = other.hitbox.x;
    hitbox.y = other.hitbox.y;
    UpdateHitbox();
    return *this;
}

GameObject GameObject::getNext()
{
    Vec2 pos = position + velocity;
    return GameObject(pos, hitbox, sprite);
}