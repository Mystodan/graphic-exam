#define TINYOBJLOADER_IMPLEMENTATION //This needs to be defined exactly once so that tinyOBJ will work


#include "camera.h"
#include "map.h"
#include "entity.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <set>
#include <cmath>

const int MSAAsamples = 20;

// -----------------------------------------------------------------------------
// ENTRY POINT
// -----------------------------------------------------------------------------
int main()
{
    // Initialization of GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed." << '\n';
        std::cin.get();

        return EXIT_FAILURE;
    }
    // Setting window hints
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    //lfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, MSAAsamples);
    // Tell glfw we are using the core profiles, meaning we only have modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int mapWidth = 5, mapHeight = 5, mapDepth = 10;
    int res = 1000;
    srand(time(NULL));
    std::ifstream inn("levels/level0");
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(16 * res, 9 * res, "BlockOut", monitor, NULL);

    if (window == nullptr) {
        std::cerr << "GLFW failed on window creation." << '\n';
        std::cin.get();

        glfwTerminate();

        return EXIT_FAILURE;
    }

    // Setting the OpenGL context.
    glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
    // Enable depht buffer 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enable MSAA
    glEnable(GL_MULTISAMPLE);

    
    Map*            level = new Map(mapWidth, mapHeight, mapDepth);
    PlayerBlock*    Player = new PlayerBlock(level);
    Camera*         camera = new Camera(16, 9, glm::vec3(level->getWidth() / 2.f, level->getHeight() / 2.f, level->getDepht()*2.f));
  
    
    // level->setcamera(camera);
    level->setCamera(camera);
    Player->setCamera(camera);
    // Specify the color of the background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        int tempwidth, tempheight;
        glfwGetFramebufferSize(window, &tempwidth, &tempheight);
        glViewport(0, 0, tempwidth, tempheight);

      
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use

       // camera->Inputs(window);

        // draws map

        
        level->draw(window);

        // draws Player
      
        Player->move(window);
        Player->draw();

   
        
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();

        if (!level->getGameStatus())
        {
            window = glfwCreateWindow(16 * res, 9 * res, "BlockOut", NULL, NULL);
            std::cout << "Game ENDED\n"; 
            delete Player;
            delete camera;
            delete level;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_SUCCESS;
}