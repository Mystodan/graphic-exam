#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;
};

struct blockVertex {
	glm::vec3 vertPos;
	float color;
	glm::vec2 texCoords;
};