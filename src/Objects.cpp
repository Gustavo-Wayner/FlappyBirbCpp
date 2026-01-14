#include "Objects.h"

GameObject::GameObject(Vec2 _position, double _width, double _height, Texture2D _sprite) : width(_width), height(_height), position(_position), sprite(_sprite)
{
    position = _position;

    hitbox.height = height;
    hitbox.width = width;
    UpdateHitbox();
}

GameObject::GameObject(const GameObject &other) : width(other.width), height(other.height), position(other.position), sprite(other.sprite)
{
    velocity = other.velocity;

    hitbox = other.hitbox;
    UpdateHitbox();
}

Vec2 GameObject::getPosition() { return position; }

void GameObject::UpdateHitbox()
{
    hitbox.x = position.x - width * 0.5f;
    hitbox.y = position.y - height * 0.5f;
}

GameObject &GameObject::operator=(const GameObject &other)
{
    width = other.width;
    height = other.height;
    position = other.position;

    velocity = other.velocity;
    sprite = other.sprite;

    hitbox = other.hitbox;
    UpdateHitbox();
    return *this;
}

GameObject GameObject::getNext()
{
    Vec2 pos = position + velocity;
    return GameObject(pos, hitbox.width, hitbox.height, sprite);
}