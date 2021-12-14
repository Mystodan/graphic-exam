#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class VBO {
private:
	// Gets byte size
	int getBytes(const std::vector<GLfloat>vertices);
	int getBytes(const std::vector<glm::vec4>vertices);

public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(std::vector<GLfloat>&vertices);
	VBO(std::vector<glm::vec4>& vertices);
	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif