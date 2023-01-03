#include "Point2.h"
#include "Vector2.h"

const Point2 Point2::Origin = Point2(0.f, 0.f);
Point2::Point2()
	: x(0), y(0)
{
}

Point2::Point2(float x, float y)
	: x(x), y(y)
{
}

float Point2::Dist(Point2 o) const
{
	return (float)std::sqrt(SQ(o.x - x) + SQ(o.y - y));
}

float Point2::DistSquared(Point2 o) const
{
	return SQ(o.x - x) + SQ(o.y - y);
}

Point2 Point2::Mid(Point2 o) const
{
	return Point2((x + o.x) / 2, (y + o.y) / 2);
}

Point2 Point2::Rotate(float angle) const
{
	float c = cos(angle);
	float s = sin(angle);

	return Point2(x * c - y * s, y * c + x * s);
}

Point2 Point2::Rotate(Point2 o, float angle) const
{
	float _x = x - o.x;
	float _y = y - o.y;

	float c = cos(angle);
	float s = sin(angle);

	return Point2(x + _x * c - _y * s, y + _y * c + _x * s);
}

Point2 Point2::Rotate(float c, float s) const
{
	return Point2(x * c - y * s, y * c + x * s);
}

Point2 Point2::Rotate(Point2 o, float c, float s) const
{
	float _x = x - o.x;
	float _y = y - o.y;

	return Point2(x + _x * c - _y * s, y + _y * c + _x * s);
}

Point2 Point2::Scale(float scale) const
{
	return Point2(x * scale, y * scale);
}

Point2 Point2::Scale(Point2 o, float scale) const
{
	float _scale = 1 - scale;

	Vector2 t(_scale * o.x, _scale * o.y);

	return Point2(scale * x, scale * y) + t;
}

Point2 Point2::Circumcenter(Point2 p1, Point2 p2, Point2 p3)
{
	float d = 2 * (
		p1.x * (p2.y - p3.y) +
		p2.x * (p3.y - p1.y) +
		p3.x * (p1.y - p2.y)
		);

	return Point2((
		(SQ(p1.x) + SQ(p1.y)) * (p2.y - p3.y) +
		(SQ(p2.x) + SQ(p2.y)) * (p3.y - p1.y) +
		(SQ(p3.x) + SQ(p3.y)) * (p1.y - p2.y)
		) / d,
		((SQ(p1.x) + SQ(p1.y)) * (p3.x - p2.x) +
		(SQ(p2.x) + SQ(p2.y)) * (p1.x - p3.x) +
		(SQ(p3.x) + SQ(p3.y)) * (p2.x - p1.x)
		) / d);
}

const Point2 Point2::IsobarycenterTriangle(Point2 p1, Point2 p2, Point2 p3)
{
	return Point2((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);
}

Point2 Point2::operator-()
{
	return Point2(-this->x, -this->y);
}

bool Point2::operator==(Point2 r)
{
	return this->x == r.x && this->y == r.y;
}

bool Point2::operator!=(Point2 r)
{
	return this->x != r.x && this->y != r.y;
}

Point2 Point2::operator+(float o)
{
	return Point2(this->x + o, this->y + o);
}

Point2 Point2::operator+(Point2 o)
{
	return Point2(this->x + o.x, this->y + o.y);
}

Point2 Point2::operator-(Point2 o)
{
	return Point2(this->x - o.x, this->y - o.y);
}

Point2 Point2::operator*(float o)
{
	return Point2(this->x * o, this->y * o);
}

Point2 Point2::operator/(float o)
{
	return Point2(this->x / o, this->y / o);
}

Point2 Point2::operator/(Point2 o)
{
	return Point2(this->x / o.x, this->y / o.y);
}

std::ostream& operator<<(std::ostream& out, Point2& p)
{
	return out << "{ " << p.x << " ; " << p.y << " }";
}
