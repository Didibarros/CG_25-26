#include <iostream>
#include <fstream>
#include <sstream>

#include "state.h"
#include "tinyxml2.h"

using namespace tinyxml2;

/*
    int width; int height;
    float position_x; float position_y; float position_z;
    float lookAt_x; float lookAt_y; float lookAt_z;
    float up_x; float up_y; float up_z;
    float fov; float near; float far;
    std::vector<Model> models;
*/

int parse3dFile(const std::string& filename, Model& model) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return -1;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Error reading file: " << filename << std::endl;
        return -1;
    }

    std::istringstream headerStream(line);
    int numVertices = 0, numTriangles = 0;
    char comma = ',';

    if (!(headerStream >> numVertices >> comma >> numTriangles)) {
        std::cerr << "Error parsing header in file: " << filename << std::endl;
        return -1;
    }
    model.vertices.reserve(numVertices);
    model.triangles.reserve(numTriangles * 3);

    for (int i = 0; i < numVertices; ++i) {
        if (!std::getline(file, line)) {
            std::cerr << "Error reading vertex data in file: " << filename << std::endl;
            return -1;
        }
        std::istringstream vertexStream(line);
        float x = 0.0f, y = 0.0f, z = 0.0f;
        if (!(vertexStream >> x >> comma >> y >> comma >> z)) {
            std::cerr << "Error parsing vertex data in file: " << filename << std::endl;
            return -1;
        }
        model.vertices.emplace_back(std::make_unique<Vertex>(x, y, z));
    }
    for (int i = 0; i < numTriangles; ++i) {
        if (!std::getline(file, line)) {
            std::cerr << "Error reading triangle data in file: " << filename << std::endl;
            return -1;
        }
        std::istringstream triangleStream(line);
        int v1 = 0, v2 = 0, v3 = 0;
        if (!(triangleStream >> v1 >> comma >> v2 >> comma >> v3)) {
            std::cerr << "Error parsing triangle data in file: " << filename << std::endl;
            return -1;
        }
        model.triangles.push_back(v1);
        model.triangles.push_back(v2);
        model.triangles.push_back(v3);
    }

    return 0;
}

int State::loadConfig(const char *config_filename)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(config_filename);
    if (err != XML_SUCCESS) {
        std::cerr << "Failed to load config.xml\n";
		return 1;
    } else {
        XMLElement* root = doc.FirstChildElement("world");
        if (!root) {
            std::cerr << "Missing 'world' element\n";
            return 1;
        }
        
		XMLElement* window = root->FirstChildElement("window");
		XMLElement* camera = root->FirstChildElement("camera");
		XMLElement* group = root->FirstChildElement("group");
		
		if (!window || !camera || !group) {
            std::cerr << "Missing required elements\n";
            return 1;
        }

		window->QueryIntAttribute("height", &(this->height));
		window->QueryIntAttribute("width", &(this->width));

		XMLElement* position = camera->FirstChildElement("position");
		if (position) {
			position->QueryFloatAttribute("x", &(this->position_x));
			position->QueryFloatAttribute("y", &(this->position_y));
			position->QueryFloatAttribute("z", &(this->position_z));
		}

		XMLElement* lookAt = camera->FirstChildElement("lookAt");
		if (lookAt) {
			lookAt->QueryFloatAttribute("x", &(this->lookAt_x));
			lookAt->QueryFloatAttribute("y", &(this->lookAt_y));
			lookAt->QueryFloatAttribute("z", &(this->lookAt_z));
		}

		XMLElement* up = camera->FirstChildElement("up");
		if (up) {
			up->QueryFloatAttribute("x", &(this->up_x));
			up->QueryFloatAttribute("y", &(this->up_y));
			up->QueryFloatAttribute("z", &(this->up_z));
		}

		XMLElement* projection = camera->FirstChildElement("projection");
		if (projection) {
			projection->QueryFloatAttribute("fov", &(this->fov));
			projection->QueryFloatAttribute("near", &(this->near));
			projection->QueryFloatAttribute("far", &(this->far));
		}

		XMLElement* xml_models = group->FirstChildElement("models");
		if (xml_models) {
			XMLElement* xml_model = xml_models->FirstChildElement();
			while (xml_model != nullptr) {
				const char* filename = xml_model->Attribute("file");
				if (filename) {
					Model model;
					if (parse3dFile(filename, model) == 0) {
						this->models.push_back(std::move(model));
					}
				}
				xml_model = xml_model->NextSiblingElement();
			}
		}
    }
	return 0;
}

void State::print() {
	std::cout << "{ 'State', \n{ 'Window', { 'Height', " << this->height << " }, { 'Width', " << this->width
		<< " } },\n{ 'Camera', { 'Position', { 'X', " << this->position_x << " }, { 'Y', " << this->position_y <<  " }, { 'Z', " << this->position_z
		<< " } }, { 'LookAt', { 'X', " << this->lookAt_x << " }, { 'Y', " << this->lookAt_y <<  " }, { 'Z', " << this->lookAt_z
		<< " } }, { 'Up', { 'X', " << this->up_x << " }, { 'Y', " << this->up_y <<  " }, { 'Z', " << this->up_z
		<< " } }, { 'Projection', { 'Fov', " << this->fov << " }, { 'Near', " << this->near <<  " }, { 'Z', " << this->far
		<< " } }\n}\n";
}

