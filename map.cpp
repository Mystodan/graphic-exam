#include "map.h"

#include <fstream>
#include <iostream>
#include <random>


/**
 *	Loads level, tiles, pellets and buffers
 *	@see loadLevel(..)
 *	@see loadTiles(..)
 *	@see loadPellets(..)
 *  @see loadBuffers(..)
 */
Map::Map(int width, int height, int depth) {
	this->width = width; this->height = height;  this->depth = depth;
	createLevel();
	loadTiles();
	loadBuffers();
}


/**
 * Cleans up memory when application closes
 */
Map::~Map() {
	g_VAO->Delete();
	g_VBO->Delete();
	g_EBO->Delete();
	mapShader->Delete();
	for (auto& b : blocks) {
		b.Delete();
	}     

}


/**
 *	Loads the level's tile types
 *	@param string - level file
 */
void Map::createLevel() {
	std::cout << "creating level ..." << std::endl;
	
	
	
		for (int i = 0; i < height * width * depth; i++) {
			t_types.push_back(1);
		}
		std::cout << "Done creating level." << std::endl;
	

};



/**
 *	Creates VAO, VBO and EBO
 *	Links verticies and indicies
 */
void Map::loadBuffers() {
	std::cout << "Generating buffers for tiles and pellets ..." << std::endl;
	mapShader = new Shader("shaders/grid.vert", "shaders/grid.frag");
	/* --- Tile buffers */
	// Generates Vertex Array Object and binds it
	g_VAO = new VAO();
	g_VAO->Bind();

	// Generates Vertex Buffer Object and links it to vertices
	g_VBO = new VBO(grid_Vert);
	// Generates Element Buffer Object and links it to indices
	g_EBO = new EBO(grid_Ind);

	// Links VBO attributes such as coordinates and colors to VAO
	g_VAO->LinkAttrib(g_VBO, 0, 3, GL_FLOAT, 4 * sizeof(float), (void*)0);
	g_VAO->LinkAttrib(g_VBO, 1, 1, GL_FLOAT, 4 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	g_VAO->Unbind();
	g_VBO->Unbind();
	g_EBO->Unbind();


	std::cout << "Done generating buffers." << std::endl;
}


/**
 *	Loads tiles based on information from level
 *	@see getTileMode(..)
 *	@see pushPellet(..)
 */
void Map::loadTiles() {
	std::cout << "Setting tiles ..." << std::endl;

	int i = 0, j = 0; // Holder styr på hvor mange tiles vi har opprettet
	for (int z = 0; z < depth; z++) 
		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++) {

				tileTyp = getTileMode(x, y, false);
				tileTypN = getTileMode(x, y + 1, false);
				tileTypE = getTileMode(x - 1, y, false);
				tileTypS = getTileMode(x, y - 1, false);
				tileTypW = getTileMode(x + 1, y, false);

				// Load tile into VBO and EBO
				pushTile(x, y, z, tileTyp, i);


			}
	
	std::cout << "Done setting tiles." << std::endl;
}





/**
 *	Gets the 2d screenspace position of a given tile.
 * 
 *	@param x - X-position.
 *	@param y - Y-position.
 *	@return The screenspace coordinates of the given tile as a pair of X and Y
 */
std::pair<float, float> Map::tileToCoords(float x, float y) {
	//std::cout << "tilex " << x << " tiley " << y << std::endl;
	return std::pair<float, float> { x, height - y -1};
}
/**
 *	Gets the 3d screenspace position of a given tile.
 *
 *	@param x - X-position.
 *	@param y - Y-position.
 * 	@param z - Z-position.
 *	@return The screenspace coordinates of the given tile as a pair of X , Y and Z
 */
glm::vec3 Map::tileToCoords(float x, float y, float z) {
	//std::cout << "tilex " << x << " tiley " << y << std::endl;
	return glm::vec3{ x, height - y - 1,z};
}

/**
 *	Gets which tile the given coordinates lays within.
 *	@param x - X-position.
 *	@param y - Y-position.
 *	@return x and y position of tile.
 */
std::pair<int, int>	Map::coordsToTile(float x, float y) {
	return std::pair<int, int> { round(x), height-1-round(y)};
}

/**
 *	Changes pellet mode when player eats it,
 *	and color changes to black.
 *
 *	@param x - X-position.
 *	@param y - Y-position.
 *	@param mode - wall/tunnel
 *	@param size - Default 1.f
 * 
 *	@see tileToCoords(x1, y1)
 */
void Map::pushTile(float x1, float y1, float z1 ,float mode, int& i, float size/*=1.f*/) {


	glm::vec3 tilePos = tileToCoords(x1, y1, z1);

	float	x = tilePos.x,
			y = tilePos.y,
			z = tilePos.z;

	int temp;
	if (tileTyp == 1) temp = 0.f;
	else temp = 1.f;

	// Bottom left 1
	grid_Vert.push_back({ tilePos.x, tilePos.y , temp , mode});
	// Bottom right 1
	grid_Vert.push_back({ tilePos.x + size ,tilePos.y ,temp, mode });
	// Top right 1
	grid_Vert.push_back({ tilePos.x + size, tilePos.y + size , temp, mode });
	// Top left 
	grid_Vert.push_back({ tilePos.x , tilePos.y + size, temp, mode });
	// Push back indices
	grid_Ind.push_back(i * 4);
	grid_Ind.push_back((i * 4) + 1);
	grid_Ind.push_back((i * 4) + 1);
	grid_Ind.push_back(i * 4   + 2);

	grid_Ind.push_back((i * 4) + 2);
	grid_Ind.push_back((i * 4) + 3);
	grid_Ind.push_back((i * 4) + 3);
	grid_Ind.push_back((i * 4));
	i++;

	if (tileTypE != 1 && tileTyp != 0) { 	// EAST
		// t_vertices.push_back({ , , , mode });
		grid_Vert.push_back({ tilePos.x,tilePos.y ,tilePos.z , mode });
		grid_Vert.push_back({ tilePos.x,tilePos.y ,tilePos.z + size , mode });
		grid_Vert.push_back({ tilePos.x,tilePos.y + size ,tilePos.z + size, mode });
		grid_Vert.push_back({ tilePos.x,tilePos.y + size ,tilePos.z , mode });


		// Push back indices
		grid_Ind.push_back(i * 4);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back(i * 4 + 2);

		i++;
	}
	if (tileTypW != 1 && tileTyp != 0) { // WEST
		grid_Vert.push_back({ tilePos.x + size, tilePos.y,tilePos.z , mode }); // WEST left 3
		grid_Vert.push_back({ tilePos.x + size, tilePos.y,tilePos.z + size , mode });	// WEST right 3
		grid_Vert.push_back({ tilePos.x + size, tilePos.y + size, tilePos.z + size, mode }); 	// WEST right 3
		grid_Vert.push_back({ tilePos.x + size,tilePos.y + size ,tilePos.z , mode }); // WEST left 3

		// Push back indices
		grid_Ind.push_back(i * 4);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back(i * 4 + 2);

	
		i++;
	}
	if (tileTypN != 1 && tileTyp != 0) { // BOTTOM
		grid_Vert.push_back({ tilePos.x,tilePos.y ,tilePos.z , mode }); // left 4
		grid_Vert.push_back({ tilePos.x + size, tilePos.y, tilePos.z, mode }); // right 4

		// Top
		grid_Vert.push_back({ tilePos.x + size, tilePos.y,tilePos.z + size , mode }); //  right 4
		grid_Vert.push_back({ tilePos.x, tilePos.y,tilePos.z + size, mode }); // left 4

		// Push back indices
		grid_Ind.push_back(i * 4);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back(i * 4 + 2);

		grid_Ind.push_back((i * 4) + 2);
		grid_Ind.push_back((i * 4) + 3);
		grid_Ind.push_back((i * 4) + 3);
		grid_Ind.push_back((i * 4));
		i++;
	}
	if (tileTypS != 1 && tileTyp != 0) {
		// Bottom left 5
		grid_Vert.push_back({ tilePos.x, tilePos.y + size,tilePos.z , mode });
		// Bottom right 5
		grid_Vert.push_back({ tilePos.x + size ,tilePos.y + size ,tilePos.z , mode });
		// Top right 5
		grid_Vert.push_back({ tilePos.x + size,tilePos.y + size ,tilePos.z + size , mode });
		// Top left 5
		grid_Vert.push_back({ tilePos.x ,tilePos.y + size ,tilePos.z + size , mode });


		// Push back indices
		grid_Ind.push_back(i * 4);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back((i * 4) + 1);
		grid_Ind.push_back(i * 4 + 2);
		grid_Ind.push_back((i * 4) + 2);
		grid_Ind.push_back((i * 4) + 3);
		grid_Ind.push_back((i * 4) + 3);
		grid_Ind.push_back((i * 4));
		i++;
	}
}




/**
 *	Returns a tile mode, when called.
 *	return -1 means out of bounds.
 * 
 *  @param x - X-position.
 *	@param y - Y-position.
 */
int Map::getTileMode(int x, int y, bool entityUse) {
	if (!entityUse) {
		if (x >= width || x < 0 || y >= height || y < 0) {
			return -1;
		}
	}
	else {
		if (x >= width || x < 0 || y >= height || y < 0) {
			return 0;
		}
	}
	//std::cout << getType()[y * width + x] << std::endl;
	return getType()[y * width + x];
}
/**
 *	Returns a tile mode, when called.
 *	return -1 means out of bounds.
 *
 *  @param x - X-position.
 *	@param y - Y-position.
 *	@param z - Y-position.
 */
int Map::getTileMode(int x, int y, int z, bool entityUse) {
	y = height - 1 - y;
	if (!entityUse) {
		if (x >= width || x < 0 || y >= height || y < 0 || z >= depth || z < 0 ) {
			return -1;
		}
	}
	else {
		if (x >= width || x < 0 || y >= height || y < 0 || z >= depth || z < 0) {
			return 0;
		}
	}
	//std::cout << getType()[y * width + x] << std::endl;
	return getType()[depth * width * x + y * depth + z];
	//return getType()[y * width + x];
}

/**
 *	sets a tile mode, when called.
 *	return -1 means out of bounds.
 *
 *  @param x - X-position.
 *	@param y - Y-position.
 *	@param z - Y-position.
 */
void Map::setTileMode(int x, int y, int z, bool state, bool entityUse) {
	
	if (!entityUse) {
		if (x >= width || x < 0 || y >= height || y < 0 || z >= depth || z < 0) {
			return;
		}
	}
	else {
		if (x >= width || x < 0 || y >= height || y < 0 || z >= depth || z < 0) {
			return;
		}
	}
	int i = depth * width * x + y * depth + z;
	t_types[i] = state;
	//return getType()[y * width + x];
}


double Map::getRandNum(int min, int max) {

	// x is in [0,1[
	double x = rand() / static_cast<double>(RAND_MAX + 1);

	// [0,1[ * (max - min) + min is in [min,max[
	double rand = min + static_cast<int>(x * (max - min));
	return rand;
}

void Map::printAllTiles() {
	for (int i = 0; i < t_types.size(); i++) {
		if (!(i % 5)) std::cout << "\n";
		std::cout << t_types[i];
	
	}
}



/**
 *	Draws tiles on screen.
 */
void Map::tileDraw() {
	g_VAO->Bind();
	glLineWidth(3.5f);
	glDrawElements(GL_LINES, sizeof(std::vector<GLuint>) + sizeof(GLuint) * grid_Ind.size(), GL_UNSIGNED_INT, 0);
}

/**
 *	Draws map items, tiles, pellets.
 *	Quits game on escape, or 0 pellets.
 *	@param window - GLFW window
 */
void Map::draw(GLFWwindow* window) {
	mapShader->Activate();
	camera->Matrix(45.0f, 0.1f, 100.0f, *mapShader, "camMatrix");

	tileDraw();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		setGameStatus();

}
