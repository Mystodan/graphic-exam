#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;
};

struct pVertex {
	glm::vec4 pelletVertices;
};