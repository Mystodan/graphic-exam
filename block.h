#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "structs.h"
#include "texture.h"
#include "camera.h"
#include "shaderClass.h"
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
    Block(int x, int y, int z) {
		
		switch (z)
		{
		case 0: color = 0; break;
		case 1: color = 1; break;
		case 2: color = 2; break;
		case 3: color = 3; break;
		case 4: color = 4; break;
		case 5: color = 5; break;
		case 6: color = 6; break;
		case 7: color = 7; break;
		case 8: color = 8; break;
		case 9: color = 9; break;
		}

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

		b_vertices.push_back({ x,y+1,z,color });
		b_vertices.push_back({ x + 1,y+1,z,color });
		b_vertices.push_back({ x + 1,y+1,z + 1,color });
		b_vertices.push_back({ x,y+1,z + 1,color });

		b_vertices.push_back({ x,y,z+1,color });
		b_vertices.push_back({ x,y+1,z+1,color });
		b_vertices.push_back({ x+1,y+1,z+1,color });
		b_vertices.push_back({ x+1,y,z+1,color });


		for (int i = 0; i < 5; i++) {
			b_indices.push_back(i * 4);
			b_indices.push_back((i * 4) + 1);
			b_indices.push_back((i * 4) + 2);
			

			b_indices.push_back((i * 4));
			b_indices.push_back((i * 4) + 2);
			b_indices.push_back((i * 4) + 3);


		}
		/* ---buffers--- */
// Generates Vertex Array Object and binds it
		BlockShader = new Shader("shaders/solidBlock.vert", "shaders/solidBlock.frag");

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

	glm::vec3 getpos() { return glm::vec3{ x,y,z }; }
	void setPos(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
	void setColor(float color) { this->color = color; }
    void draw(Camera* camera) {
		BlockShader->Activate();
		camera->Matrix(45.0f, 0.1f, 100.0f, *BlockShader, "camMatrix");
		vao->Bind();
		glLineWidth(2.f);
		glDrawElements(GL_TRIANGLES, sizeof(std::vector<GLuint>) + sizeof(GLuint) * b_indices.size(), GL_UNSIGNED_INT, 0);
    }
    std::vector<glm::vec4> b_vertices;
    std::vector<GLuint> b_indices;

private:
	Texture* BlockTexture;
    Shader* BlockShader;
    VBO* vbo;
    VAO* vao;
    EBO* ebo;
	float x, y, z, color;
};

#endif