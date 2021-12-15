#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "structs.h"
#include "vao.h"
#include "ebo.h"
#include "vbo.h"

class Block {
public:
/**
 *	Loads in blocks verticies to make cube with color
 *	Loads and links VAO, VBO and EBO
 *  @params x - sets posx
 *  @params y - sets posy
 *  @params z - sets posz
 *  @params color - sets color
 */
    Block(int x, int y, int z, float color) {
		b_vertices.push_back({ x,y,z,color });
		b_vertices.push_back({ x + 1,y,z,color });
		b_vertices.push_back({ x + 1,y,z + 1,color });
		b_vertices.push_back({ x,y,z + 1,color });

		b_vertices.push_back({ x + 1,y,z,color });
		b_vertices.push_back({ x + 1,y,z + 1,color });
		b_vertices.push_back({ x + 1,y + 1,z + 1,color });
		b_vertices.push_back({ x + 1,y + 1,z,color });

		b_vertices.push_back({ x,y,z,color });
		b_vertices.push_back({ x,y,z + 1,color });
		b_vertices.push_back({ x,y + 1,z + 1,color });
		b_vertices.push_back({ x,y + 1,z,color });

		b_vertices.push_back({ x,y + 1,z,color });
		b_vertices.push_back({ x + 1,y + 1,z,color });
		b_vertices.push_back({ x + 1,y + 1,z + 1,color });
		b_vertices.push_back({ x,1,z + 1,color });

		for (int i = 0; i < 4; i++) {
			b_indices.push_back(i * 4);
			b_indices.push_back((i * 4) + 1);
			b_indices.push_back((i * 4) + 1);
			b_indices.push_back(i * 4 + 2);

			b_indices.push_back((i * 4) + 2);
			b_indices.push_back((i * 4) + 3);
			b_indices.push_back((i * 4) + 3);
			b_indices.push_back((i * 4));

		}
		/* ---buffers--- */
// Generates Vertex Array Object and binds it
		vao = new VAO();
		vao->Bind();

		// Generates Vertex Buffer Object and links it to vertices
		vbo = new VBO(b_vertices);
		// Generates Element Buffer Object and links it to indices
		ebo = new EBO(b_indices);

		// Links VBO attributes such as coordinates and colors to VAO
		vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 4 * sizeof(float), (void*)0);
		vao->LinkAttrib(vbo, 1, 1, GL_FLOAT, 4 * sizeof(float), (void*)(3 * sizeof(float)));
		// Unbind all to prevent accidentally modifying them
		vao->Unbind();
		vbo->Unbind();
		ebo->Unbind();

    }


	void setPos(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
	void setColor(float color) { this->color = color; }
    void draw() {
		vao->Bind();
		glLineWidth(2.f);
		glDrawElements(GL_LINES, sizeof(std::vector<GLuint>) + sizeof(GLuint) * b_indices.size(), GL_UNSIGNED_INT, 0);
    }
    std::vector<glm::vec4> b_vertices;
    std::vector<GLuint> b_indices;

private:
    VBO* vbo;
    VAO* vao;
    EBO* ebo;
	float x, y, z, color;
};

#endif