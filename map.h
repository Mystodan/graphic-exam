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
	VAO*				t_VAO;
	VBO*				t_VBO;
	EBO*				t_EBO;
	
	std::vector<glm::vec4>  t_vertices;
	std::vector<GLuint>     t_indices;
	std::vector<GLuint>	    t_types;

	// map tiles
	VAO* f_VAO;
	VBO* f_VBO;
	EBO* f_EBO;

	std::vector<float>  f_vertices;
	std::vector<GLuint> f_indices;
	std::vector<GLuint>	f_types;

	// pellets
	VAO*				p_VAO;
	VBO*				p_VBO;
	EBO*				p_EBO;

	std::vector<int>	pelletID;
	std::vector<bool>   hasPellet;
	std::vector<float>	p_vertices;
	std::vector<GLuint> p_indices;

	


public:
	std::vector<GLuint> getType() { return t_types; }
	std::vector<bool> checkPellet() { return hasPellet; }
	int  getGameStatus(){ return gameOn; }
	int  getWidth()		{ return width; }
	int  getHeight()	{ return height; }
	int  getDepht()	{ return depth; }
	int  getPelletAmount() { return gPellets; }
	int  getPelletIndices() { return p_indices.size(); }
	int  getTileMode(int x, int y, bool entityUse = true);
	int  getTileMode(int x, int y, int z, bool entityUse = true);
glm::vec3 
		 getSpawn()	{ return spawnTile; }
  double getRandNum(int min, int max);


	void printAllTiles();
	void setTileMode(int x, int y, int z, bool state = false, bool entityUse = true);
	void setSpawn(glm::vec3 newPos)	{ spawnTile = newPos; }
	void setGameStatus(bool mode = false) { this->gameOn = mode; }
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