#include "Vector2.h"
#include "Point2.h"

const Vector2 Zero = Vector2(0.f, 0.f);
const Vector2 UnitY = Vector2(0.f, 1.f);
const Vector2 UnitX = Vector2(1.f, 0.f);

Vector2::Vector2()
	: x(0), y(0)
{
}

Vector2::Vector2(float x, float y)
	: x(x), y(y)
{
}

Vector2::Vector2(Point2 p)
	: x(p.x), y(p.y)
{
}

Vector2::Vector2(Point2 p1, Point2 p2)
	: x(p2.x - p1.x), y(p2.y - p1.y)
{
}

float Vector2::GetNorm() const
{
	return sqrt(x * x + y * y);
}

float Vector2::GetSquaredNorm() const
{
	return x * x + y * y;
}

Vector2 Vector2::Normalize() const
{
	float norm = GetNorm();
	return Vector2(x / norm, y / norm);
}

Vector2 Vector2::GetNormal() const
{
	return Vector2(y, -x).Normalize();
}

float Vector2::DotProduct(Vector2 o) const
{
	return x * o.x + y * o.y;
}

float Vector2::Angle() const
{
	return atan2f(y, x);
}

float Vector2::Angle(Vector2 o) const
{
	float dotProduct = DotProduct(o);
	float angle = acosf(
		dotProduct / (GetNorm() * o.GetNorm())
	);

	if (Determinant(o) < 0)
		angle = -angle;

	return angle;
}

float Vector2::Determinant(Vector2 o) const
{
	return (x * o.y) - (o.x * y);
}

Vector2 Vector2::operator+(const Vector2 o)
{
	return Vector2(this->x + o.x, this->y + o.y);
}

Vector2 Vector2::operator-(const Vector2 o)
{
	return Vector2(this->x - o.x, this->y - o.y);
}

Vector2 Vector2::operator-()
{
	return Vector2(-this->x, -this->y);
}

Vector2 Vector2::operator*(const Vector2 o)
{
	return Vector2(this->x * o.x, this->y * o.y);
}

Vector2 Vector2::operator*(float s)
{
	return Vector2(this->x * s, this->y * s);
}

Vector2 Vector2::operator/(const Vector2 o)
{
	return Vector2(this->x / o.x, this->y / o.y);
}

Vector2 Vector2::operator/(float s)
{
	return Vector2(this->x / s, this->y / s);
}

bool Vector2::operator<(const Vector2 o)
{
	return this->GetNorm() < o.GetNorm();
}

bool Vector2::operator<(float s)
{
	return this->GetNorm() < s;
}

bool Vector2::operator>(const Vector2 o)
{
	return this->GetNorm() > o.GetNorm();
}

bool Vector2::operator>(float s)
{
	return this->GetNorm() > s;
}

Point2 operator+(Point2 l, Vector2 r)
{
	return Point2(l.x + r.x, l.y + r.y);
}

Point2 operator-(Point2 l, Vector2 r)
{
	return Point2(l.x - r.x, l.y - r.y);
}

Point2 operator*(Point2 l, Vector2 r)
{
	return Point2(l.x * r.x, l.y * r.y);
}

Point2 operator/(Point2 l, Vector2 r)
{
	return Point2(l.x / r.x, l.y / r.y);
}

std::ostream& operator<<(std::ostream& out, Vector2& v)
{
	return out << "{ " << v.x << " ; " << v.y << " }";
}
