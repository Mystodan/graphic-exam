#include "entity.h"
#include "block.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <math.h>
/** 
 *	Creates an entity, loads in initial values
 *	@param x - X-position.
 *	@param y - Y-position.
 *	@param level - link to level
 */
Entity::Entity(Map* level) {
	std::cout << "Setting entity..." << std::endl;
	this->map = level;
					// generating buffers for player
};
/**
 *	Deconstructor
 */
Entity::~Entity() {
	vao->Delete();
	vbo->Delete();
	ebo->Delete();
	
};

/**
 *	Creates a player, loads in initial values
 *	@param x - X-position.
 *	@param y - Y-position.
 *	@param level - link to level
 */
PlayerBlock::PlayerBlock(Map* level, int speed/*=0.02f*/) : Entity(level) {
	glm::vec3
		Pos = map->tileToCoords(level->getWidth()/2, level->getHeight()/2, level->getDepht()-1);
	spawnPosX = Pos.x;
	spawnPosY = Pos.y;
	spawnPosZ = Pos.z+0.99;
	resetPos();
	gravityCounter = std::chrono::steady_clock::now();
	shader = new Shader("shaders/playerBlock.vert", "shaders/playerBlock.frag");
	// Set player spawn
	color = 3;
	constSpeed = speed;
	// remember to add texture and shit.
	std::cout << "Generating buffers for player ..." << std::endl;
	loadBuffers();
};

/**
 *	Memory clean up on close
 */
PlayerBlock::~PlayerBlock() {
	shader->Delete();
};
/**
 *	Takes the Player position and resets it
 *	resets the position.
 */
void Entity::resetPos() {
	posX = spawnPosX;
	posY = spawnPosY;
	posZ = spawnPosZ;
}

/**
 *	Loads in player verticies to make 1x1 tile and color
 *	Loads and links VAO, VBO and EBO
 */
void Entity::loadBuffers() {
	float x = 0,
		  y = 0,
		  z = -1;
	// in xyz
	// x + 0, x + 0, x + 0
	// x + 1, y, z,
	// x + 1, y, z + 1
	vertices.push_back({ x,y,z,color });
	vertices.push_back({ x+1,y,z,color });
	vertices.push_back({ x+1,y,z+1,color });
	vertices.push_back({ x,y,z+1,color });

	vertices.push_back({ x+1,y,z,color });
	vertices.push_back({ x+1,y,z+1,color });
	vertices.push_back({ x+1,y+1,z+1,color });
	vertices.push_back({ x+1,y+1,z,color });

	vertices.push_back({ x,y,z,color });
	vertices.push_back({ x,y,z+1,color });
	vertices.push_back({ x,y+1,z+1,color });
	vertices.push_back({ x,y+1,z,color });

	vertices.push_back({ x,y+1,z,color });
	vertices.push_back({ x+1,y+1,z,color });
	vertices.push_back({ x+1,y+1,z+1,color });
	vertices.push_back({ x,1,z+1,color });

	for (int i = 0; i < 4; i++) {
		indices.push_back(i * 4);
		indices.push_back((i * 4) + 1);
		indices.push_back((i * 4) + 1);
		indices.push_back(i * 4 + 2);

		indices.push_back((i * 4) + 2);
		indices.push_back((i * 4) + 3);
		indices.push_back((i * 4) + 3);
		indices.push_back((i * 4));
	
	}

	/* ---buffers--- */
	// Generates Vertex Array Object and binds it
	vao = new VAO();
	vao->Bind();

	// Generates Vertex Buffer Object and links it to vertices
	vbo = new VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	ebo = new EBO(indices);

	// Links VBO attributes such as coordinates and colors to VAO
	vao->LinkAttrib(std::move(vbo), 0, 3, GL_FLOAT, 4 * sizeof(float), (void*)0);
	vao->LinkAttrib(std::move(vbo), 1, 1, GL_FLOAT, 4 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	vao->Unbind();
	vbo->Unbind();
	ebo->Unbind();
}

/**
 *	Loads in blocks verticies to make 1x1 tile and color
 *	Loads and links VAO, VBO and EBO
 */
void Entity::createSolidBlocks() {
		Block block(posX, posY, posZ);
		map->blocks.push_back(block);
}

/**
 *	Gets tile mode and returns it
 *	@return tile mode (wall/tunnel/player)
 */
int Entity::getTileMode(int x, int y) {

	return map->getTileMode(x, y);
};
int Entity::getTileMode(int x, int y, int z) {
	return map->getTileMode(x, y, z);
};

void Entity::setTileMode(int x, int y, int z) {
	map->setTileMode(x, y, z);
};


int Entity::checkSolidBlock() {

	// Gets and saves current tile
	glm::vec3 tile = map->tileToCoords(posX, posY, posZ),
		tempTile;
	// saves modes as temp values
	int tempMode1,
		tempMode2,
		wall = -1;
	tile.z -= 0.001f;
	
	tempTile.x = tile.x;
	tempTile.y = tile.y;
	tempTile.z = tile.z - 1;
	tempMode1 = getTileMode(floor(tile.x), floor(tile.y),floor(tile.z));
	tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));

	if (tempMode1 == tempMode2) { return tempMode1; 	
	} // returns walkable tile if possible
	else { 
		return wall; }
}

/**
 *	Checks for tile and pellet collision
 *	@param facing - direction
 *	@return The mode of the tile in front of us
 */
int Entity::checkFacingTile(direction facing) {
	// Gets and saves current tile
	glm::vec3 tile = map->tileToCoords(posX, posY, posZ),
		tempTile;

	// saves modes as temp values
	int tempMode1,
		tempMode2,
		wall = -1;

	// player
	tile.x += 0.001; tile.y += 0.001;
	// Update it with facing dir
	switch (facing) {
	case left: {
		tile.x -= 0.005f;				// moves player along the x(.x) value
		
		tempTile.x = tile.x; 			// checks tile to the left
		tempTile.y = tile.y + 0.99f;
		tempTile.z = tile.z;

		tempMode1 = getTileMode(floor(tile.x), floor(tile.y),floor(tile.z));
		tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));
		if (tempMode1 == tempMode2) return tempMode1; // returns walkable tile if possible
		else return wall;

		break; }

	case right: {
		tile.x += 1.005f;					// moves player along the x(.x) value
		
		tempTile.x = tile.x;			// checks tile to the right
		tempTile.y = tile.y + 0.99f;
		tempTile.z = tile.z;
		tempMode1 = getTileMode(floor(tile.x), floor(tile.y), floor(tile.z));
		tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));

		if (tempMode1 == tempMode2) return tempMode1; // returns walkable tile if possible
		else return wall;

		break; }

	case up: {
		tile.y -= 0.005f;					// moves player along the x(.x) value
		
		tempTile.x = tile.x + 0.99f;   // checks tile above
		tempTile.y = tile.y;
		tempTile.z = tile.z;
		tempMode1 = getTileMode(floor(tile.x), floor(tile.y), floor(tile.z));
		tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));
		if (tempMode1 == tempMode2) return tempMode1; // returns walkable tile if possible
		else return wall;

		break; }

	case down: {
		tile.y += 1.005f;  // moves player along the x(.x) value
		
		tempTile.x = tile.x + 0.99f;    // checks tile under
		tempTile.y = tile.y;
		tempTile.z = tile.z;
		tempMode1 = getTileMode(floor(tile.x), floor(tile.y), floor(tile.z));
		tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));
		if (tempMode1 == tempMode2) return tempMode1; // returns walkable tile if possible
		else return wall;

		break; }

	}
};

/**
 *	Moves player based on player input,
 *	and checks for collison.
 *	@param GLFW window
 */
void PlayerBlock::move(GLFWwindow* window) {
	dt = glfwGetTime();
	//std::cout << posX << ", " << posY << ", " << posZ << std::endl;
	// Get relevant keys
	bool k_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS,
		 k_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS,
		 k_up = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS,
		 k_down = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS,
		 k_space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

	// Check which direction the player wants to turn
	direction newDir = (k_left ? left :
		k_right ? right :
		k_up ? up :
		k_down ? down :
		none);

	// Check if we can actually turn towards newdir, 
	// if so change speed.

	if (checkFacingTile(newDir) || !singleStep) {
		facing = newDir; speed = constSpeed; 
	}
	if (checkFacingTile(newDir) == 0) {
		speed = 0;
	}

	if (tpDelay) {
		tp_eTT = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(tp_eTT - tp_Count).count() > 0.5 * 1000.f) {
			if (k_space) {
						posZ -= speed;
						roundPos(1, 1, 1);
			}
			tpDelay = false;
		}	
	}
	else {
		if (k_space) {
		posZ -= speed;
	
		}
	}

	if (firstMove) {
		
		gravityTT = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(gravityTT - gravityCounter).count() >= 3) {
			gravityCounter = std::chrono::steady_clock::now();
			posZ -= 1;
			roundPos(1, 1, 1);
		}
	}
	 

		
	float tempSpeed = constSpeed/10;

	if (!checkSolidBlock()) {
		
		tempSpeed = 0; roundPos(1, 1, 1); 
		if (posZ < 8.8f) { //checks player pos
		map->setTileMode(posX, posY, posZ); // creates solid blocks 
		createSolidBlocks();	
		}

		else if(posZ > 8.1f) { //if player has a block bellow itself and its z = 9 or higher?? yeah you lose then
			map->setGameStatus();// sets game to lose   
		}
		
		tp_Count = std::chrono::steady_clock::now();
		tpDelay = true;
		resetPos();
	
	}
	else {
	
	}
	
	 //create solid block at z = 0

	// Flips firstMove to true when player starts moving
	if (k_left + k_right + k_up + k_down == 1)	firstMove = true;

	// Go in that direction 
	int reduceBy = 1200;
	switch (facing) {
	case left: {	 
		if (singleStep && checkFacingTile(newDir) == 1) {
			posX -= speed; 
		
			turnCounter = std::chrono::steady_clock::now(); 
			singleStep = false;  
			facing = none;  break;
		}
	}
	case right: {	
		if (singleStep && checkFacingTile(newDir) == 1) {
			posX += speed;
			
			turnCounter = std::chrono::steady_clock::now();
			singleStep = false;
			facing = none;  break;
		}
	}
	case up:	{	 
		if (singleStep && checkFacingTile(newDir) == 1) {
			posY += speed;
		
			turnCounter = std::chrono::steady_clock::now();
			singleStep = false;
			facing = none; break;
		}
	}
	case down:	{	 
		if (singleStep && checkFacingTile(newDir) == 1) {
			posY -= speed;
		
			turnCounter = std::chrono::steady_clock::now();
			singleStep = false;
			facing = none;  break;
		}
	}
	}

	if (!singleStep) {
		
		endTurnTime = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(endTurnTime - turnCounter).count() > 0.1 * 1000.f) {
			singleStep = true;
		}
	}

	// Updates player position
	Entity::updatePos();
};

void PlayerBlock::updatePos() {
	Entity::updatePos();

}

double Entity::getRandNum(int min, int max) {

	// x is in [0,1[
	double x = rand() / static_cast<double>(RAND_MAX + 1);

	// [0,1[ * (max - min) + min is in [min,max[
	double rand = min + static_cast<int>(x * (max - min));
	return rand;
}


/**
 *	Updates player posistion when called
 *	Transforms player posistion
 */
void Entity::updatePos() {
	shader->Activate();
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(posX, posY, posZ));
	transform = glm::rotate(transform, 0.0f/*(GLfloat)glfwGetTime() * -5.0f*/, glm::vec3(0.0f, 0.0f, 1.0f));
	camera->Matrix(45.0f, 0.1f, 100.0f, *shader, "transformPac", transform);
};

/**
 *	Sets camera when called
 */
void Entity::setCamera(Camera* camera) {
	this->camera = camera;
};

/**
 *	Draws player on screen
 */
void Entity::draw() {
	vao->Bind();
	glLineWidth(2.f);
	glDrawElements(GL_LINES, sizeof(std::vector<GLuint>) + sizeof(GLuint) * indices.size(), GL_UNSIGNED_INT, 0);
	drawSolidBlocks();
}
void Entity::drawSolidBlocks() {

	for (auto& blocks : map->blocks) {
		blocks.draw(camera);
	
	}
	

}