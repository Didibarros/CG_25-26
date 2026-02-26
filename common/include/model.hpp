#ifndef PROJECTO_SHAPE_H
#define PROJECTO_SHAPE_H

#include <vector>
#include <memory>
#include <string>
#include "point.hpp"

struct Model
{
	std::vector<std::unique_ptr<Vertex>> vertices;
    std::vector<int> triangles; // Define os triângulos do modelo. _ % 3 = 0.
	void clear();
	void pushTriangle(int v1, int v2, int v3);
	void generateTriangle(float p1, float p2, float p3);
	void generatePlane(float dimension, int divisions);
	void generateBox(float dimension, int divisions);
	void generateCone(float radius, float height, int slices, int stacks);
	void generateSphere(float radius, int slices, int stacks);
	void writeToFile(const std::string& filename);
};

#endif