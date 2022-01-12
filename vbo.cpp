#include "vbo.h"
#include <vector>
#include <iostream>
/**
 *	Constructor that generates a Vertex Buffer Object and links it to vertices
 */
VBO::VBO(std::vector<glm::vec4>& vertices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, getBytes(vertices), &vertices[0], GL_STATIC_DRAW);
}
/**
 *	Constructor that generates a Vertex Buffer Object and links it to vertices
 */
VBO::VBO(std::vector<GLfloat>&vertices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, getBytes(vertices), &vertices[0], GL_STATIC_DRAW);
}
/**
 *	Constructor that generates a Vertex Buffer Object and links it to vertices
 */
VBO::VBO(std::vector<blockVertex>& vertices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, getBytes(vertices), &vertices[0], GL_STATIC_DRAW);
}
/**
 *	Binds the VBO
 */
void VBO::Bind() {
 	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

/**
 *	Unbinds the VBO
 */
void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/** 
 *	Deletes the VBO
 */
void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}

/**
 *	gets bytes of our data type
 *	@return size - of vector
 */
int VBO::getBytes(const std::vector<GLfloat>vertices) {
	return sizeof(GLfloat)*vertices.size();
}
/**
 *	gets bytes of our data type
 *	@return size - of vector
 */
int VBO::getBytes(const std::vector<glm::vec4>vertices) {
	return sizeof(glm::vec4) * vertices.size();
}
/**
 *	gets bytes of our data type
 *	@return size - of vector
 */
int VBO::getBytes(const std::vector<blockVertex>vertices) {
	return sizeof(blockVertex) * vertices.size();

}
