#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <imgui.h>

#define SQ(var) ((var) * (var))

class Point2
{
public:
	float x;
	float y;

	static const Point2 Origin;

	Point2();

	Point2(ImVec2 pos);

	Point2(float xy);

	Point2(float x, float y);

	float Dist(Point2 o) const;

	float DistSquared(Point2 o) const;

	Point2 Mid(Point2 o) const;

	Point2 Rotate(float angle) const;

	Point2 Rotate(Point2 o, float angle) const;

	Point2 Rotate(float c, float s) const;

	Point2 Rotate(Point2 o, float c, float s) const;

	Point2 Scale(float scale) const;

	Point2 Scale(Point2 o, float scale) const;

	static Point2 Circumcenter(Point2 p1, Point2 p2, Point2 p3);

	static const Point2 IsobarycenterTriangle(Point2 p1, Point2 p2, Point2 p3);

	Point2 operator-();

	Point2 operator+(float o);
	Point2 operator+(Point2 o);
	Point2 operator-(Point2 o);
	Point2 operator*(float o);
	Point2 operator/(float o);
	Point2 operator/(Point2 o);

	bool operator==(Point2 o);

	bool operator!=(Point2 o);

	operator ImVec2() { return ImVec2(x, y); }

	friend std::ostream& operator<<(std::ostream& out, Point2& p);

};
