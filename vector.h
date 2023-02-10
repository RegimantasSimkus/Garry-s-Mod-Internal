#pragma once

typedef float vec_t;

struct ImVec2;

class Vector
{
public:
	vec_t x, y, z;

	Vector(void)
	{
		x = y = z = 0;
	}

	Vector(vec_t X, vec_t Y = 0.f, vec_t Z = 0.f)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vector& Init()
	{
		x = y = z = 0;
		return *this;
	}

	inline vec_t LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}


	inline Vector& operator=(const Vector& vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	inline vec_t& operator[](int i)
	{
		return ((vec_t*)this)[i];
	}

	inline vec_t operator[](int i) const
	{
		return ((vec_t*)this)[i];
	}

	inline Vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	operator ImVec2();
};

class VectorAligned : public Vector
{
public:
	float w;
};

inline void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

inline void VectorMultiply(const Vector& a, vec_t b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

inline void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

inline void VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.0f;
}

inline void VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}