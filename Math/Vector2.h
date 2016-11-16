#pragma once

template <class T>
class Vector2
{
public:
	Vector2()
		: x(0), y(0)
	{
	}

	Vector2(T x, T y)
		: x(x), y(y)
	{
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}

	Vector2& operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	T Length() const
	{
		return sqrt(x * x + y * y);
	}

	T LengthSq() const
	{
		return x * x + y * y;
	}

	T Dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2 Normalize() const
	{
		T length = sqrt(x * x + y * y);
		T inv_length = 1 / length;
		return Vector2(x * inv_length, y * inv_length);
	}

	T x, y;
};

typedef Vector2<double> Vector2d;
typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;