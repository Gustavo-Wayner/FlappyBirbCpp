#include "Objects.h"

GameObject::GameObject(Vec2 _position, Rectangle _hitbox, Texture2D _sprite) : position(_position), sprite(_sprite)
{
    hitbox.width = _hitbox.width;
    hitbox.height = _hitbox.height;
    hitbox.x = _hitbox.x;
    hitbox.y = _hitbox.y;
    position = _position;

    UpdateHitbox();
}

GameObject::GameObject(const GameObject &other) : position(other.position), sprite(other.sprite)
{
    velocity = other.velocity;

    hitbox.width = other.hitbox.width;
    hitbox.height = other.hitbox.height;
    hitbox.x = other.hitbox.x;
    hitbox.y = other.hitbox.y;
    UpdateHitbox();
}

Vec2 GameObject::getPosition() { return position; }

void GameObject::UpdateHitbox(const float& offsetX, const float& offsetY)
{
    if(velocity.Magnitude() == 0) return;
    hitbox.x = position.x + (velocity.x - hitbox.width * 0.5f + offsetX);
    hitbox.y = position.y + (velocity.y - hitbox.height * 0.5f + offsetY);
}

GameObject &GameObject::operator=(const GameObject &other)
{
    position = other.position;

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