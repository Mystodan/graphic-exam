#ifndef player_H
#define player_H

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
	left, right, up, down,depth, none
};

class Entity {
	public:
		bool					tpDelay = false;
		double					dt;
		float					posX,
								posY,
								posZ,
								spawnPosX,
								spawnPosY,
								spawnPosZ,
								speed,
								constSpeed,
								color;
		direction				facing;
		int						nextTileX,
								nextTileY;
		Map*					map;
		Camera*					camera;
		VAO*					vao;
		VBO*					vbo;
		EBO*					ebo;
		std::vector<glm::vec4>  vertices;
		std::vector<GLuint>		indices;
		Shader*					shader;
		
		Entity(Map* level);
		~Entity();
		
		std::pair<float, float> getPos() { return std::pair<float, float> { posX, posY }; }
		void roundPos(bool x = false, bool y = false, bool z = false) { if (x) { posX = round(posX); } if (y) { posY = round(posY); } if (z) { posZ = round(posZ); } }
		void setCamera(Camera* camera);
		void setTileMode(int x, int y, int z);

		int dropTimer = 2; //dropTimer
		int  checkFacingTile(direction facing);
		int  checkSolidBlock();
		void updatePos();
		void draw();
		void loadBuffers();
		int  getTileMode(int x, int y);
		int  getTileMode(int x, int y, int z);
		double  getRandNum(int min, int max);

		void resetPos();
		void createSolidBlocks();
		void drawSolidBlocks();
		std::chrono::steady_clock::time_point				tp_Count,
															turnCounter,
															gravityCounter;
		std::chrono::time_point<std::chrono::steady_clock>	tp_eTT,
															endTurnTime,
															gravityTT;
};



/**
 *
 */
class PlayerBlock : public Entity {
private:
	bool	isAlive = true,
			firstMove = false,
			singleStep = false;
	void	updatePos();
public:
	void setIsAlive(bool state = false) { this->isAlive = state; }
	bool getIsAlive() { return this->isAlive; }
	bool hasMoved() { return this->firstMove; }
	void move(GLFWwindow* window);
	PlayerBlock(Map* level, int speed = 1);
	~PlayerBlock();
};




#endif
