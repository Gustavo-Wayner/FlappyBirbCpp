#pragma once

#include <raylib.h>
#include <cmath>

struct Vec2
{
    static float Dot(const Vec2 &vecA, const Vec2 &vecB);
    static float Angle(const Vec2 &vecA, const Vec2 &vecB);
    static void LimitMagnitude(Vec2 &vec, float max, float min = 0);

    float Magnitude() const;
    Vec2 GetUnitVector() const;
    void Normalize();
    void SetMagnitude(float scalar);

    float x;
    float y;
    Vec2() = default;
    Vec2(float _x, float _y);
    Vec2(float arr[2]);

#pragma region operator=

    Vec2(const Vec2 &) = default;
    Vec2 &operator=(const Vec2 &other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    Vec2 &operator=(float (&other)[2])
    {
        this->x = other[0];
        this->y = other[1];

        return *this;
    }
#pragma endregion

#pragma region operator*
    Vec2 operator*(float scalar) const
    {
        return {x * scalar, y * scalar};
    }

    friend Vec2 operator*(float scalar, const Vec2 &vec)
    {
        return {vec.x * scalar, vec.y * scalar};
    }

    Vec2 &operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
#pragma endregion

#pragma region operator/
    Vec2 operator/(float scalar) const
    {
        return {x / scalar, y / scalar};
    }

    friend Vec2 operator/(float scalar, const Vec2 &vec)
    {
        return {vec.x / scalar, vec.y / scalar};
    }

    Vec2 &operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
#pragma endregion

#pragma region operator+
    Vec2 operator+(float other[2])
    {
        return {this->x + other[0], this->y + other[1]};
    }

    Vec2 operator+(Vec2 other)
    {
        return {this->x + other.x, this->y + other.y};
    }

    Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }

    Vec2 &operator+=(float other[2])
    {
        this->x += other[0];
        this->y += other[1];

        return *this;
    }

    Vec2 &operator+=(Vec2 other)
    {
        this->x += other.x;
        this->y += other.y;

        return *this;
    }

#pragma endregion

#pragma region operator -
    Vec2 operator-(float other[2])
    {
        return {this->x - other[0], this->y - other[1]};
    }

    Vec2 operator-(const Vec2 &other) { return {this->x - other.x, this->y - other.y}; }

    Vec2 &operator-=(const Vec2 &other)
    {
        this->x -= other.x;
        this->y -= other.y;

        return *this;
    }

    Vec2 &operator-=(float other[2])
    {
        this->x -= other[0];
        this->y -= other[1];

        return *this;
    }
#pragma endregion
};

inline int sign(float num) { return ((num > 1) - (num < 1)); }
inline float absolute(float num) { return num * sign(num); }