#include "entity.h"

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
					// generating buffers for pacman
};
Entity::Entity() {
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
 *	Creates a pacman, loads in initial values
 *	@param x - X-position.
 *	@param y - Y-position.
 *	@param level - link to level
 */
Pacman::Pacman(Map* level, int speed/*=0.02f*/) : Entity(level) {
	glm::vec3
		Pos = map->tileToCoords(0, 0, level->getDepht()-1);
	spawnPosX = Pos.x;
	spawnPosY = Pos.y;
	spawnPosZ = Pos.z;
	resetPos();

	shader = new Shader("shaders/player.vert", "shaders/player.frag");
	// Set player spawn
	color = 3;
	constSpeed = speed;
	// remember to add texture and shit.
	std::cout << "Generating buffers for Pacman ..." << std::endl;
	loadBuffers();
};

/**
 *	Memory clean up on close
 */
Pacman::~Pacman() {
	shader->Delete();
};
/**
 *	Creates a ghost, loads in initial values
 *	@param x - X-position.
 *	@param y - Y-position.
 *	@param level - link to level
 *	@param target - set entity target of who to eat
 *	@param colour - set the colour of the ghost
 *	@param speed - set the speed of the ghost
 */
Ghost::Ghost( Map* level, Pacman* target, float colour/*=0.f*/, float speed/*=0.02f*/) : Entity(level) {
	shader = new Shader("shaders/ghost.vert", "shaders/ghost.frag");
	Target = target;
	color = colour;
	constSpeed = speed;
	
	// sets facing of ghosts on startup
	double startVal = 0, endVal = 1, randnum = getRandNum(startVal, endVal);
	(randnum > startVal) ? facing = left : facing = right;
	if (checkFacingTile(facing) == 1) { (randnum > startVal) ?  facing = down : facing = up;}

	/*facing = venstre;
	if (checkFacingTile(facing) == 1) {
		facing = ned;
	}*/
	velX = 0;
	velY = 0;
	// remember to add texture and shit.
	std::cout << "Generating buffers for Ghost ..." << std::endl;
	loadBuffers(); // 0, 0, 0
};

Ghost::Ghost() {
};
/**
 *	Memory clean up on close
 */
Ghost::~Ghost() {
	shader->Delete();
};
void Entity::resetPos() {
	posX = spawnPosX;
	posY = spawnPosY;
	posZ = spawnPosZ;
}

/**
 *	Loads in pacMan verticies to make 1x1 tile and color
 *	Loads and links VAO, VBO and EBO
 */
void Entity::loadBuffers() {
	float x = 0,
		  y = 0,
		  z = 0;
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
void Entity::drawSolidBlock(float x, float y, float z, int i) {
	glm::vec3 vertice;
	glm::vec3 normal;
	glm::vec2 texCoord;

	vertice = { x, y, z };
	normal = { 1.f, 1.f, 1.f };
	texCoord = { 0.f, 0.f };
	tex_vert.push_back({ vertice, normal, texCoord });

	vertice = { x + 1.f, y, z };
	normal = { 1.f, 1.f, 1.f };
	texCoord = { 0.f, 1.f };
	tex_vert.push_back({ vertice, normal, texCoord });

	vertice = { x + 1.f, y + 1.f, z };
	normal = { 1.f, 1.f, 1.f };
	texCoord = { 1.f, 1.f };
	tex_vert.push_back({ vertice, normal, texCoord });

	vertice = { x, y + 1.f, z };
	normal = { 1.f, 1.f, 1.f };
	texCoord = { 1.f, 0.f };
	tex_vert.push_back({ vertice, normal, texCoord });
	// in xyz
	// x + 0, x + 0, x + 0
	// x + 1, y, z,
	// x + 1, y, z + 1
	b_indices.push_back(i * 4);
	b_indices.push_back((i * 4) + 1);
	b_indices.push_back((i * 4) + 2);
	b_indices.push_back(i * 4);
	b_indices.push_back((i * 4) + 2);
	b_indices.push_back((i * 4) + 3);
	i++;

	/* ---buffers--- */
	// Generates Vertex Array Object and binds it
	b_vao = new VAO();
	b_vao->Bind();

	// Generates Vertex Buffer Object and links it to vertices
	b_vbo = new VBO(tex_vert);
	// Generates Element Buffer Object and links it to indices
	b_ebo = new EBO(b_indices);
	blockShader = new Shader("shaders/tile.vert", "shaders/tile.frag");
	blockTexture = new Texture("resources/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	blockTexture->texUnit(blockShader, "tex0", 0);


	// Links VBO attributes such as coordinates and colors to VAO
	b_vao->LinkAttrib(b_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	b_vao->LinkAttrib(b_vbo, 1, 1, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	b_vao->Unbind();
	b_vbo->Unbind();
	b_ebo->Unbind();
	
	// Tells OpenGL which Shader Program we want to use
	blockShader->Activate();
	// Exports the camera Position to the Fragment Shader for specular lighting
	glUniform3f(glGetUniformLocation(blockShader->ID, "camPos"), camera->Position.x, camera->Position.y, camera->Position.z);
	// Export the camMatrix to the Vertex Shader of the pyramid
	camera->Matrix(blockShader, "camMatrix");
	// Binds texture so that is appears in rendering
	blockTexture->Bind();
	// Bind the VAO so OpenGL knows to use it
	b_vao->Bind();
	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, sizeof(std::vector<GLuint>) + sizeof(GLuint) * b_indices.size(), GL_UNSIGNED_INT, 0);


}

/**
 *	Gets tile mode and returns it
 *	@return tile mode (wall/tunnel/pacman)
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

	tile.z -= 0.005f;
	tempTile.x = tile.x;
	tempTile.y = tile.y;
	tempTile.z = tile.z;
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

	// Pacman
	tile.x += 0.001; tile.y += 0.001;
	// Update it with facing dir
	switch (facing) {
	case left: {
		tile.x -= 0.005f;				// moves pacman along the x(.x) value
		
		tempTile.x = tile.x; 			// checks tile to the left
		tempTile.y = tile.y + 0.99f;
		tempTile.z = tile.z;

		tempMode1 = getTileMode(floor(tile.x), floor(tile.y),floor(tile.z));
		tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));
		if (tempMode1 == tempMode2) return tempMode1; // returns walkable tile if possible
		else return wall;

		break; }

	case right: {
		tile.x += 1.005f;					// moves pacman along the x(.x) value
		
		tempTile.x = tile.x;			// checks tile to the right
		tempTile.y = tile.y + 0.99f;
		tempTile.z = tile.z;
		tempMode1 = getTileMode(floor(tile.x), floor(tile.y), floor(tile.z));
		tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));

		if (tempMode1 == tempMode2) return tempMode1; // returns walkable tile if possible
		else return wall;

		break; }

	case up: {
		tile.y -= 0.005f;					// moves pacman along the x(.x) value
		
		tempTile.x = tile.x + 0.99f;   // checks tile above
		tempTile.y = tile.y;
		tempTile.z = tile.z;
		tempMode1 = getTileMode(floor(tile.x), floor(tile.y), floor(tile.z));
		tempMode2 = getTileMode(floor(tempTile.x), floor(tempTile.y), floor(tempTile.z));
		if (tempMode1 == tempMode2) return tempMode1; // returns walkable tile if possible
		else return wall;

		break; }

	case down: {
		tile.y += 1.005f;  // moves pacman along the x(.x) value
		
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
 *	Moves pacman based on player input,
 *	and checks for collison.
 *	@param GLFW window
 */
void Entity::move(GLFWwindow* window) {
	dt = glfwGetTime();
	//std::cout << posX << ", " << posY << ", " << posZ << std::endl;
	// Get relevant keys
	bool				k_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS,
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
	if (k_space) { posZ -= 0.1f; }
	float tempSpeed = constSpeed/8;
	if (!checkSolidBlock()) {
		
		tempSpeed = 0; roundPos(1, 1, 1); 

		map->setTileMode(posX, posY, posZ);
		drawSolidBlock(posX, posY, posZ, blockCount);
		resetPos();
	
	};
	
	 //create solid block at z = 0

	// Flips firstMove to true when player starts moving
	if (k_left + k_right + k_up + k_down == 1)	firstMove = true;

	// Go in that direction 
	int reduceBy = 1000;
	switch (facing) {
	case left: {	 
		if (singleStep && checkFacingTile(newDir) == 1) {
			posX -= speed; 
			posZ -= tempSpeed * dt / reduceBy; 
			turnCounter = std::chrono::steady_clock::now(); 
			singleStep = false;  
			facing = none;  break;
		}
	}
	case right: {	
		if (singleStep && checkFacingTile(newDir) == 1) {
			posX += speed;
			posZ -= tempSpeed * dt / reduceBy;
			turnCounter = std::chrono::steady_clock::now();
			singleStep = false;
			facing = none;  break;
		}
	}
	case up:	{	 
		if (singleStep && checkFacingTile(newDir) == 1) {
			posY += speed;
			posZ -= tempSpeed * dt / reduceBy;
			turnCounter = std::chrono::steady_clock::now();
			singleStep = false;
			facing = none; break;
		}
	}
	case down:	{	 
		if (singleStep && checkFacingTile(newDir) == 1) {
			posY -= speed;
			posZ -= tempSpeed * dt / reduceBy;
			turnCounter = std::chrono::steady_clock::now();
			singleStep = false;
			facing = none;  break;
		}
	}
	case none: posZ -= tempSpeed * dt / reduceBy; break;
	}

	if (!singleStep) {
		endTurnTime = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(endTurnTime - turnCounter).count() > 0.1 * 1000.f) {
			singleStep = true;
		}
	}

	// Updates pacman position
	Entity::updatePos();
};

void Pacman::updatePos() {
	Entity::updatePos();

}

double Ghost::getRandNum(int min, int max) {

	// x is in [0,1[
	double x = rand() / static_cast<double>(RAND_MAX + 1);

	// [0,1[ * (max - min) + min is in [min,max[
	double rand = min + static_cast<int>(x * (max - min));
	return rand;
}



void Ghost::checkPac() {
	std::pair<float, float> pacPos = Target->getPos();	
	std::pair<float,float> ghostPos = map->coordsToTile(posX,posY);
	if (round(pacPos.first) == round(ghostPos.first) && round(pacPos.second) == round(posY)) {
		eatPac();
	}
}
void Ghost::updatePos() {
	Entity::updatePos();
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(posX, posY, 0.0f));
	transform = glm::rotate(transform, 0.0f/*(GLfloat)glfwGetTime() * -5.0f*/, glm::vec3(0.0f, 0.0f, 1.0f));
	camera->Matrix(45.0f, 0.1f, 100.0f, *shader, "transformGhost", transform);
}


/**
 *	Updates Pacman posistion when called
 *	Transforms Pacman posistion
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
 *	Draws pacman on screen
 */
void Entity::draw() {
	vao->Bind();
	glLineWidth(2.f);
	glDrawElements(GL_LINES, sizeof(std::vector<GLuint>) + sizeof(GLuint) * indices.size(), GL_UNSIGNED_INT, 0);
}
