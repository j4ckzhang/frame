#pragma once
#include <cmath>

template <class T>
class Vector3
{
public:

	Vector3()
		: x(0), y(0), z(0)
	{
	}

	Vector3(T x, T y, T z)
		: x(x), y(y), z(z)
	{
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator-() const
	{
		return Vector3(- x, -y, -z);
	}

	Vector3& operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	T Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	T LengthSq() const
	{
		return x * x + y * y + z * z;
	}

	T Dot(const Vector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3 Cross(const Vector3& other) const
	{
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	T Distance(const Vector3& other) const
	{
		Vector3 v = *this - other;
		return v.Length();
	}

	Vector3 Normalize() const
	{
		T length = sqrt(x * x + y * y + z * z);
		T inv_length = 1 / length;
		return Vector3(x * inv_length, y * inv_length, z * inv_length);
	}

	T x, y, z;
};

typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;