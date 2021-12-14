#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class EBO {
private:
	// Gets byte size
	int getBytes(const std::vector<GLuint>indices);
	int getBytes(const std::vector<glm::vec4>indices);

public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(std::vector<GLuint> &indices);
	EBO(std::vector<glm::vec4> &indices);
	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};

#endif