#include <iostream>
#include <cstring>
#include "utils.hpp"
#include "model.hpp"

using namespace std;

const char* instructions = "Usage:\n - 'plane {length} {divisions} {filename}'\n - 'box {length} {divisions} {filename}'\n - 'sphere {radius} {slices} {stacks} {filename}'\n - 'cone {radius} {height} {filename}'\n";

int main(const int argc, char** argv){
	if (argc < 2) {
		std::cerr << "Error: No arguments provided.\n" << instructions;
		return 1;
	}

	std::string shapeType = argv[1];

	if(shapeType != "plane" && shapeType != "box" && shapeType != "sphere" && shapeType != "cone"){
		std::cerr << "Error: Invalid shape type '" << shapeType << "'.\n" << instructions;
		return 1;
	}
	if (shapeType == "plane") {
		if (argc != 5) {
			std::cerr << "Error: Invalid number of arguments for plane.\n";
			return 1;
		}
		Model model;
		model.generatePlane(stof(argv[2]), stoi(argv[3]));
		model.writeToFile(argv[4]);
	}
	if (shapeType == "box") {
		if (argc != 5) {
			std::cerr << "Error: Invalid number of arguments for box.\n";
			return 1;
		}
		Model model;
		model.generateBox(stof(argv[2]), stoi(argv[3]));
		model.writeToFile(argv[4]);
	}
	if (shapeType == "sphere") {
		if (argc != 6) {
			std::cerr << "Error: Invalid number of arguments for sphere.\n";
			return 1;
		}
		Model model;
		model.generateSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]));
		model.writeToFile(argv[5]);
	}
	if (shapeType == "cone") {
		if (argc != 7) {
			std::cerr << "Error: Invalid number of arguments for cone.\n";
			return 1;
		}
		Model model;
		model.generateCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]));
		model.writeToFile(argv[6]);
	}
	return 0;
}
