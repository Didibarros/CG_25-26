#ifndef PROJECTO_STATE_H
#define PROJECTO_STATE_H

#include <string>
#include <vector>
#include "model.hpp"

struct State
{
    // position
    int width;
    int height;
    // position
    float position_x;
    float position_y;
    float position_z;
    // lookAt
    float lookAt_x;
    float lookAt_y;
    float lookAt_z;
    // up
    float up_x;
    float up_y;
    float up_z;
    // projection
    float fov;
    float near;
    float far;
    // models
    std::vector<Model> models;
    int loadConfig(const char *config_filename);
	void print();
};

#endif