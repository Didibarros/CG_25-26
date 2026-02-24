//
// Created by helder on 19/02/26.
//

#include "point.hpp"
#include "utils.hpp"

Point::Point() { }
Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
Point::Point(const Point& other) {
	this->x; other.x;
	this->y = other.y;
	this->z = other.z;
}