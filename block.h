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
	Block(int x, int y, int z);
    void Delete() {
        vbo->Delete();
        vao->Delete();
        ebo->Delete();
        BlockTexture->Delete();
        BlockShader->Delete();
    }
	glm::vec3 getpos() { return glm::vec3{ x,y,z }; }
	void setPos(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
	void setColor(float color) { this->color = color; }
    void draw(Camera* camera);
    std::vector<blockVertex> b_vertices;
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