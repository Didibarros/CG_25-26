#include <iostream>

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

int State::loadConfig(const char *config_filename)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(config_filename);
    if (err != XML_SUCCESS) {
        std::cerr << "Failed to load config.xml\n";
		return 1;
    } else {
        XMLElement* root = doc.FirstChildElement("world");
		XMLElement* window = root->FirstChildElement("window");
		XMLElement* camera = root->FirstChildElement("camera");
		XMLElement* group = root->FirstChildElement("group");

		window->QueryIntAttribute("height", &(this->height));
		window->QueryIntAttribute("width", &(this->width));

		XMLElement* position = camera->FirstChildElement("position");
		position->QueryFloatAttribute("x", &(this->position_x));
		position->QueryFloatAttribute("y", &(this->position_y));
		position->QueryFloatAttribute("z", &(this->position_z));

		XMLElement* lookAt = camera->FirstChildElement("lookAt");
		lookAt->QueryFloatAttribute("x", &(this->lookAt_x));
		lookAt->QueryFloatAttribute("y", &(this->lookAt_y));
		lookAt->QueryFloatAttribute("z", &(this->lookAt_z));

		XMLElement* up = camera->FirstChildElement("up");
		up->QueryFloatAttribute("x", &(this->up_x));
		up->QueryFloatAttribute("y", &(this->up_y));
		up->QueryFloatAttribute("z", &(this->up_z));

		XMLElement* projection = camera->FirstChildElement("projection");
		projection->QueryFloatAttribute("fov", &(this->fov));
		projection->QueryFloatAttribute("near", &(this->near));
		projection->QueryFloatAttribute("far", &(this->far));

		XMLElement* xml_models = group->FirstChildElement("models");
		XMLElement* xml_model = xml_models->FirstChildElement();
		while (xml_model != nullptr) {
			const char* filename = xml_model->Attribute("file");
			xml_model = xml_model->NextSiblingElement();
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