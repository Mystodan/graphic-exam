#include "block.h"
/**
* Creates a block based on position
* @param x - x position 
* @param y - y position 
* @param z - z position 
*/
Block::Block(int x, int y, int z) {

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

	b_vertices.push_back({ glm::vec3{x,y,z},color,glm::vec2{0,0} }); //bottom
	b_vertices.push_back({ glm::vec3{x + 1,y,z},color,glm::vec2{0,1} });
	b_vertices.push_back({ glm::vec3{x + 1,y,z + 1},color ,glm::vec2{1,1} });
	b_vertices.push_back({ glm::vec3{x,y,z + 1},color,glm::vec2{1,0} });

	b_vertices.push_back({ glm::vec3{x + 1,y,z},color ,glm::vec2{0,0} });//right
	b_vertices.push_back({ glm::vec3{x + 1,y,z + 1},color,glm::vec2{1,0} });
	b_vertices.push_back({ glm::vec3{x + 1,y + 1,z + 1},color,glm::vec2{1,1} });
	b_vertices.push_back({ glm::vec3{x + 1,y + 1,z},color,glm::vec2{0,1} });

	b_vertices.push_back({ glm::vec3{x,y,z},color ,glm::vec2{0,0} });//left
	b_vertices.push_back({ glm::vec3{x,y,z + 1},color,glm::vec2{1,0} });
	b_vertices.push_back({ glm::vec3{x,y + 1,z + 1},color,glm::vec2{1,1} });
	b_vertices.push_back({ glm::vec3{x,y + 1,z},color,glm::vec2{0,1} });

	b_vertices.push_back({ glm::vec3{x,y + 1,z},color,glm::vec2{1,0} }); // top
	b_vertices.push_back({ glm::vec3{x + 1,y + 1,z},color ,glm::vec2{1,1} });
	b_vertices.push_back({ glm::vec3{x + 1,y + 1,z + 1},color,glm::vec2{0,1} });
	b_vertices.push_back({ glm::vec3{x,y + 1,z + 1},color,glm::vec2{0,0} });

	b_vertices.push_back({ glm::vec3{x,y,z + 1},color,glm::vec2{0,0} });
	b_vertices.push_back({ glm::vec3{x,y + 1,z + 1},color,glm::vec2{1,0} });//front
	b_vertices.push_back({ glm::vec3{x + 1,y + 1,z + 1},color,glm::vec2{1,1} });
	b_vertices.push_back({ glm::vec3{x + 1,y,z + 1},color,glm::vec2{0,1} });


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
	BlockTexture = new Texture("resources/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	BlockShader = new Shader("shaders/solidBlock.vert", "shaders/solidBlock.frag");
	BlockTexture->Bind();

	vao = new VAO();
	vao->Bind();

	// Generates Vertex Buffer Object and links it to vertices
	vbo = new VBO(b_vertices);
	// Generates Element Buffer Object and links it to indices
	ebo = new EBO(b_indices);

	// Links VBO attributes such as coordinates and colors to VAO
	vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	vao->LinkAttrib(vbo, 1, 1, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, 6 * sizeof(float), (void*)(4 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	vao->Unbind();
	vbo->Unbind();
	ebo->Unbind();

}

/**
* Draws a block
* @param camera - camera 
*/
void Block::draw(Camera* camera) {
	BlockShader->Activate();
	camera->Matrix(45.0f, 0.1f, 100.0f, *BlockShader, "camMatrix");
	vao->Bind();
	glDrawElements(GL_TRIANGLES, sizeof(GLuint) * b_indices.size(), GL_UNSIGNED_INT, 0);
}