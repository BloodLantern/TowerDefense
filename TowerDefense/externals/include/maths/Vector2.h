#pragma once

#include "Point2.h"

class Vector2
{
public:
	float x;
	float y;

	static const Vector2 Zero;
	static const Vector2 UnitY;
	static const Vector2 UnitX;

	Vector2();

	Vector2(float xy);

	Vector2(float x, float y);

	Vector2(Point2 p);

	Vector2(Point2 p1, Point2 p2);

	float GetNorm() const;

	float GetSquaredNorm() const;

	Vector2 Normalize() const;

	Vector2 GetNormal() const;

	float DotProduct(Vector2 o) const;

	float Angle() const;

	float Angle(Vector2 o) const;

	float Determinant(Vector2 o) const;

	Vector2 operator+(const Vector2 o);

	Vector2 operator+=(const Vector2 o);

	Vector2 operator-(const Vector2 o);

	Vector2 operator-=(const Vector2 o);

	Vector2 operator-();

	Vector2 operator*(const Vector2 o);

	Vector2 operator*(float ro);

	Vector2 operator/(const Vector2 o);

	Vector2 operator/(float s);

	bool operator<(const Vector2 o);

	bool operator<(float s);

	bool operator>(const Vector2 o);

	bool operator>(float s);
	
	operator ImVec2() { return ImVec2(x, y); }

	friend Point2 operator+(Point2 l, Vector2 r);
	friend Point2 operator-(Point2 l, Vector2 r);
	friend Point2 operator*(Point2 l, Vector2 r);
	friend Point2 operator/(Point2 l, Vector2 r);
		
	friend std::ostream& operator<<(std::ostream& out, Vector2& v);
};
