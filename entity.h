#ifndef PACMAN_H
#define PACMAN_H

#include <iostream>
#include <vector>
#include <string>
#include <time.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "shaderClass.h"
#include "map.h"
#include "camera.h"
#include <chrono>


enum direction {
	left, right, up, down, none
};

class Entity {
	private:
		bool				isAlive = true,
							firstMove = false,
							singleStep = false;
		// lights
		VAO*				lightVAO;
		VBO*				lightVBO;
		EBO*				lightEBO;
		Shader*				lightShader;
		GLuint              l_ID;


		std::vector<Vertex>	l_vertices;
		std::vector<GLuint>	l_indices;
	public:
		double					dt;
		float					posX,
								posY,
								posZ,
								spawnPosX,
								spawnPosY,
								spawnPosZ,
								velX,
								velY,
								velZ,
								speed,
								constSpeed,
								color;
		direction				facing;
		int						nextTileX,
								nextTileY,
								blockCount = 0;
		Map*					map;
		Camera*					camera;
		VAO*					vao,
		   *					b_vao;
		VBO*					vbo,
		   *					b_vbo;
		EBO*					ebo,
		   *					b_ebo;
		std::vector<glm::vec4>  vertices;
		std::vector<Vertex>		tex_vert;
		std::vector<GLuint>		indices;
		std::vector<GLuint>		b_indices;
		Shader*					shader,
			  *	    			blockShader;
		GLuint					b_ID;
		Texture*				blockTexture;
		
		Entity(Map* level);
		Entity();
		~Entity();
		
		std::pair<float, float> getPos() { return std::pair<float, float> { posX, posY }; }
		void roundPos(bool x = false, bool y = false, bool z = false) { if (x) { posX = round(posX); } if (y) { posY = round(posY); } if (z) { posZ = round(posZ); } }
		void setCamera(Camera* camera);
		void setIsAlive(bool state = false) { this->isAlive = state; }
		int  checkFacingTile(direction facing);
		int  checkSolidBlock();
		void updatePos();
		void draw();
		void loadBuffers();
		int  getTileMode(int x, int y);
		int  getTileMode(int x, int y, int z);
		void setTileMode(int x, int y, int z);
		void move(GLFWwindow* window);
		bool getIsAlive() { return this->isAlive; }
		bool hasMoved() { return this->firstMove; }
		void resetPos();
		void createSolidBlock(float x, float y, float z, int i);
		void drawSolidBlock();
		std::chrono::steady_clock::time_point				turnCounter;
		std::chrono::time_point<std::chrono::steady_clock>	endTurnTime;
};



/**
 *
 */
class PlayerBlock : public Entity {
private:
	void updatePos();
public:
	PlayerBlock(Map* level, int speed = 1);
	~PlayerBlock();
};

class Ghost : public Entity{

private:
	// player target
	PlayerBlock*				Target;
	std::vector<float>			vertices;
	std::vector<GLuint>			indices;
	void updatePos();
	void bounce();
	double  getRandNum(int min, int max);
	void checkPac();
	void eatPac() { map->setGameStatus(); };

public:
	Ghost( Map* level, PlayerBlock* target, float colour = 0.f, float speed = 0.02f );
	Ghost();
	~Ghost();
};




#endif
