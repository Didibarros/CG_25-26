#ifndef PROJECTO_POINT_H
#define PROJECTO_POINT_H

#include <memory>
#include <string>

class Point
{
public:
	float x, y, z;
	Point();
	Point(float x, float y, float z);
	Point(const Point& other);
};

typedef Point Vertex;

#endif