#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include "ebo.h"
#include "vao.h"
#include "vbo.h"
#include "shaderClass.h"
#include "block.h"

#include "texture.h"
#include "camera.h"
#include "stb_image.h"


/**
 *	A class for loading and displaying the map
 */
class Map {
private:
	
	unsigned int		width, 
						height,
						depth,
						gPellets = 0;

	bool				gameOn = true;
	int					tempwidth, 
						tempheight, 
						resolution,
						tileTyp,
						tileTypN,
						tileTypE,
						tileTypW,
						tileTypS;
	glm::vec3			spawnTile;


	// map tiles
	VAO*				g_VAO; //grid
	VBO*				g_VBO;
	EBO*				g_EBO;

	std::vector<glm::vec4>  grid_Vert;
	std::vector<GLuint>     grid_Ind;
	// storage for tile types
	std::vector<GLuint>	    t_types;


	


public:

	Shader* mapShader;
	Camera* camera;

	std::vector<Block>	    blocks;
	std::vector<GLuint> getType() { return t_types; }

	int  getGameStatus(){ return gameOn; }
	int  getWidth()		{ return width; }
	int  getHeight()	{ return height; }
	int  getDepht()	{ return depth; }


	int  getTileMode(int x, int y, bool entityUse = true);
	int  getTileMode(int x, int y, int z, bool entityUse = true);
glm::vec3 
		 getSpawn()	{ return spawnTile; }
  double getRandNum(int min, int max);


	void printAllTiles();
	void setTileMode(int x, int y, int z, bool state = false, bool entityUse = true);
	void setSpawn(glm::vec3 newPos)	{ spawnTile = newPos; }
	void setGameStatus(bool mode = false) { this->gameOn = mode; }
	void setCamera(Camera* camera) {this->camera = camera;}
	std::pair<float, float> tileToCoords(float x, float y);
	glm::vec3 tileToCoords(float x, float y, float z);
	std::pair<int, int>		coordsToTile(float x, float y);
	
	void pushTile(float x1, float y1, float z1, float mode, int& i,  float size=1.f);

	void createLevel();
	void loadTiles();
	void loadBuffers();

	void tileDraw();
	void draw(GLFWwindow* window);

	Map(int width, int height, int depth);
	~Map();
};



#endif