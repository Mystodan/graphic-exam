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
								nextTileY;
		Map*					map;
		Camera*					camera;
		VAO*					vao,
		   *  					b_vao;
		VBO*					vbo,
		   *					b_vbo;
		EBO*					ebo,
		   *					b_ebo;
		std::vector<glm::vec4>  vertices,
								b_vertices;
		std::vector<GLuint>		indices,
								b_indices;
		Shader*					shader;
		
		Entity(Map* level);
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
		void createSolidBlocks(float x, float y, float z);
		void drawSolidBlocks();
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
	std::vector<float>  vertices;
	std::vector<GLuint> indices;
	void updatePos();
	void bounce();
	double  getRandNum(int min, int max);
	void checkPac();
	void eatPac() { map->setGameStatus(); };

public:
	Ghost( Map* level, PlayerBlock* target, float colour = 0.f, float speed = 0.02f );
	~Ghost();
};




#endif
