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
    
    int width = 0, height = 0, res = 40;
    srand(time(NULL));
    std::ifstream inn("levels/level0");
    if (inn) { inn >> width; inn.ignore(1); inn >> height; }
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* screenMode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(width * res, height * res, "Pacman", NULL, NULL);

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
    // Enable MSAA
    glEnable(GL_MULTISAMPLE);


    Map*    level0 = new Map(5,5,10);
    PlayerBlock* Player = new PlayerBlock(level0);

    Shader* mapShader = new Shader("shaders/default.vert", "shaders/default.frag");

    Camera* camera = new Camera(width, height, glm::vec3(level0->getWidth() / 2.f, level0->getHeight() / 2.f, level0->getDepht()*2.f));
    Player->setCamera(camera);


    
  
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // Scale with aspect ratio
        int tempwidth, tempheight;
        glfwGetFramebufferSize(window, &tempwidth, &tempheight);
        glViewport(0, 0, tempwidth, tempheight);
        glfwSetWindowAspectRatio(window, screenMode->height, screenMode->height);

        // Specify the color of the background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use

        camera->Inputs(window);

        // draws map
        mapShader->Activate();
        camera->Matrix(45.0f, 0.1f, 100.0f, *mapShader, "camMatrix");
        
        level0->draw(window);

        // draws Player
      
        Player->move(window);
        Player->draw();

   
        
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();

        if (!level0->getGameStatus())
        {
            break;
        }
    }

    return EXIT_SUCCESS;
}