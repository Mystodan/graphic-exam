#include "EBO.h"

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(std::vector<GLuint> &indices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getBytes(indices), &indices[0], GL_STATIC_DRAW);
}
// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(std::vector<glm::vec4>& indices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getBytes(indices), &indices[0], GL_STATIC_DRAW);
}

// Binds the EBO
void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}

int EBO::getBytes(const std::vector<GLuint>indices) {
	return sizeof(std::vector<GLuint>) + sizeof(GLfloat) * indices.size();
}

int EBO::getBytes(const std::vector<glm::vec4>indices) {
	return sizeof(std::vector<GLuint>) + sizeof(GLfloat) * indices.size();
}