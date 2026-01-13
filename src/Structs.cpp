#include "Structs.h"

#pragma region Vector2
float Vec2::Dot(const Vec2 &vecA, const Vec2 &vecB)
{
    return vecA.x * vecB.x + vecA.y * vecB.y;
}
float Vec2::Magnitude() const { return sqrt(Dot(*this, *this)); }
float Vec2::Angle(const Vec2 &vecA, const Vec2 &vecB)
{
    return acos(Vec2::Dot(vecA, vecB) / (vecA.Magnitude() * vecB.Magnitude()));
}
void Vec2::LimitMagnitude(Vec2 &vec, float max, float min)
{
    if (vec.Magnitude() > max)
        vec.SetMagnitude(max);
    else if (vec.Magnitude() < min)
        vec.SetMagnitude(min);
}

Vec2 Vec2::GetUnitVector() const { return *this / Magnitude(); }

void Vec2::Normalize() { *this /= Magnitude(); }

void Vec2::SetMagnitude(float scalar)
{
    Normalize();
    *this *= scalar;
}

Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {}
Vec2::Vec2(float arr[2]) : x(arr[0]), y(arr[1]) {}
#pragma endregion