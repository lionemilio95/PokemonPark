#include "window.h"
#include "Model.hpp"
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "libirrklang.dylib") // link with irrKlang.dll

const char* window_title = "GLFW Starter Project";


Cube * cube;
Model * squirtle;
Model * squirtleSphere;

Model * bulbasaur;
Model * bulbasaurSphere;

Terrain * testTerrain;
Lsystem * tree1;

GLint isSquirtleBulbasaurCollide = 0;


GLint shaderProgram;
GLint ToonshaderProgram;
GLint SphereshaderProgram;
GLint TerrainshaderProgram;
ISoundEngine* engine;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

#define TOON_VERTEX_SHADER_PATH "Toonshader.vert"
#define TOON_FRAGMENT_SHADER_PATH "Toonshader.frag"

#define SPHERE_VERTEX_SHADER_PATH "Sphereshader.vert"
#define SPHERE_FRAGMENT_SHADER_PATH "Sphereshader.frag"

#define TERRAIN_VERTEX_SHADER_PATH "Terrainshader.vert"
#define TERRAIN_FRAGMENT_SHADER_PATH "Terrainshader.frag"
#define TIME_LIMIT 200

// Default camera parameters
glm::vec3 cam_pos(0.0f, 50.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

//Cursor callback variables
double oldX, oldY;
int origWidth;
int origHeight;
int mouseClick = 0;
int firstMouse = 1;

// Switch for toon shading
int isToon = 0;

// Debug mode for bounding sphere
int isSphere = 0;

int greenOnTimer = 0;


glm::mat4 Window::P;
glm::mat4 Window::V;

glm::vec3 trackBallMapping(double x, double y);

void Window::initialize_objects()
{
    
    
    
    //testTerrain = new Terrain();
    testTerrain = new Terrain(0.25, 47, -37, 0.55, 50, 50);
	
    tree1 = new Lsystem("tree1.l");
    
    
    
    
    // Create and modify squirtle
    squirtle = new Model("squirtle-animation-test-obj/squirtle-animation-test.obj");
    
    glm::mat4 squirtle_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
    squirtle->toWorld = squirtle_rotation * squirtle->toWorld;
    
    glm::mat4 squirtle_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.5f, 0.0f));
    squirtle->toWorld = squirtle_translate * squirtle->toWorld;
    
    
    // Create and modify bounding sphere for squirtle
    squirtleSphere = new Model("sphere_2.obj");
    glm::mat4 squirtleSphere_scale = glm::mat4(glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f)));
    squirtleSphere->toWorld = squirtleSphere_scale * squirtleSphere->toWorld;
    
    glm::mat4 squirtleSphere_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.1f, -0.7f));
    squirtleSphere->toWorld = squirtleSphere_translate * squirtleSphere->toWorld;
    

    // Create and modify bulbasaur
    bulbasaur = new Model("bulbasaur-pokemon-go-obj/bulbasaur-pokemon-go.obj");
    
    glm::mat4 bulbasaur_scale = glm::mat4(glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 0.6f)));
    bulbasaur->toWorld = bulbasaur_scale * bulbasaur->toWorld;
    
    glm::mat4 bulbasaur_translate = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -2.0f, 0.0f));
    bulbasaur->toWorld = bulbasaur_translate * bulbasaur->toWorld;
    
    // Create and modify bulbasaurSphere
    bulbasaurSphere = new Model("sphere_2.obj");
    
    glm::mat4 bulbasaurSphere_scale = glm::mat4(glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f)));
    bulbasaurSphere->toWorld = bulbasaurSphere_scale * bulbasaurSphere->toWorld;
    
    glm::mat4 bulbasaurSphere_translate = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -0.2f, 0.1f));
    bulbasaurSphere->toWorld = bulbasaurSphere_translate * bulbasaurSphere->toWorld;
    
    
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    ToonshaderProgram = LoadShaders(TOON_VERTEX_SHADER_PATH, TOON_FRAGMENT_SHADER_PATH);
    SphereshaderProgram = LoadShaders(SPHERE_VERTEX_SHADER_PATH, SPHERE_FRAGMENT_SHADER_PATH);
    TerrainshaderProgram = LoadShaders(TERRAIN_VERTEX_SHADER_PATH, TERRAIN_FRAGMENT_SHADER_PATH);
    //set BGM by YUAN WANG
    engine = createIrrKlangDevice();
    if (!engine)
    {
        printf("Could not startup Sound engine\n");
        return ; // error starting up the engine
    }
    engine->play2D("Opening.mp3", true);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	//delete(cube);
    delete(squirtle);
    delete(squirtleSphere);
    delete(bulbasaur);
    delete(bulbasaurSphere);
    
	glDeleteProgram(shaderProgram);
    glDeleteProgram(ToonshaderProgram);
    glDeleteProgram(SphereshaderProgram);
    glDeleteProgram(TerrainshaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);
    
    origWidth = width;
    origHeight = height;

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
//    squirtle->update();
//    bulbasaur->update();
//    squirtleSphere->update();
//    bulbasaurSphere->update();
    
    
    greenOnTimer --;
    if (greenOnTimer < 0)greenOnTimer = 0;
    
    if(squirtleSphere->CollisonBool(bulbasaurSphere) == true){
        
        greenOnTimer = TIME_LIMIT;
        engine->play2D("bump.mp3");

        isSquirtleBulbasaurCollide = 1;
        squirtle->turnDegree(90);
        squirtleSphere->turnDegree(90);
        bulbasaur->turnDegree(90);
        bulbasaurSphere->turnDegree(90);
        
        //std::cout << "collide!" << std::endl;
    }else if (greenOnTimer > 0){
        isSquirtleBulbasaurCollide = 1;
    }else{
        //std::cout << "Not collide..." << std::endl;
        isSquirtleBulbasaurCollide = 0;
    }
    
    squirtle->moveForward();
    squirtleSphere->moveForward();
    //bulbasaur->moveForward();
    //bulbasaurSphere->moveForward();

    
    //std::cout << isSquirtleBulbasaurCollide << std::endl;
    
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(isToon == 0){
    // Use the shader of programID
        glUseProgram(shaderProgram);
        squirtle->Draw(shaderProgram);
        bulbasaur->Draw(shaderProgram);
    }else{
        glUseProgram(ToonshaderProgram);

        GLuint inlightDir = glGetUniformLocation(ToonshaderProgram, "lightDir");
        glm::vec3 mylightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
        glUniform3fv(inlightDir, 1, &mylightDir[0]);

        GLuint incam_pos = glGetUniformLocation(ToonshaderProgram, "cam_pos");
        glUniform3fv(incam_pos, 1, &cam_pos[0]);

        squirtle->Draw(ToonshaderProgram);
        bulbasaur->Draw(shaderProgram);
    }

    if(isSphere == 1){
        glUseProgram(SphereshaderProgram);
        
        GLuint inisSquirtleBulbasaurCollide = glGetUniformLocation(SphereshaderProgram, "isSquirtleBulbasaurCollide");
        glUniform1i(inisSquirtleBulbasaurCollide, isSquirtleBulbasaurCollide);
        
        squirtleSphere->Draw(SphereshaderProgram);
        bulbasaurSphere->Draw(SphereshaderProgram);
    }
    glUseProgram(TerrainshaderProgram);
    testTerrain->draw(TerrainshaderProgram);
    


	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        
        // use 'T' to switch on and off for Toon shading
        if (key == GLFW_KEY_T)
        {
            if(isToon == 1){
                isToon = 0;
            }
            else{
                isToon = 1;
            }
        }
        if(key == GLFW_KEY_UP){
            squirtle->moveForward();
            squirtleSphere->moveForward();
            //bulbasaur->moveForward();
            //bulbasaurSphere->moveForward();
            
            
        }
        
        if (key == GLFW_KEY_DOWN){
            squirtle->moveBackward();
            squirtleSphere->moveBackward();
            //bulbasaur->moveBackward();
            //bulbasaurSphere->moveBackward();
            
        }
        
        if (key == GLFW_KEY_LEFT){
            squirtle->moveLeft();
            squirtleSphere->moveLeft();
            //bulbasaur->moveLeft();
            //bulbasaurSphere->moveLeft();
            
        }
        if (key == GLFW_KEY_RIGHT){
            squirtle->moveRight();
            squirtleSphere->moveRight();
            //bulbasaur->moveRight();
            //bulbasaurSphere->moveRight();
            
        }
        
        if (key == GLFW_KEY_Z){
            engine->play2D("jump.mp3");

            // turn left 90 degree
            squirtle->turnDegree(90.0);
            squirtleSphere->turnDegree(90.0);
            //bulbasaur->turnDegree(90.0);
            //bulbasaurSphere->turnDegree(90.0);
        }
        if (key == GLFW_KEY_X){
            engine->play2D("jump.mp3");

            
            // turn right 90 degree
            squirtle->turnDegree(-90.0);
            squirtleSphere->turnDegree(-90.0);
            //bulbasaur->turnDegree(-90.0);
            //bulbasaurSphere->turnDegree(-90.0);
            
        }
        
        if (key == GLFW_KEY_W)
        {
            glm::vec3 cam_dir = - cam_pos + cam_look_at;
            cam_dir = glm::normalize(cam_dir);
            cam_pos = cam_pos + cam_dir;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
            
        }
        
        if (key == GLFW_KEY_A)
        {
            glm::vec3 cam_dir = - cam_pos + cam_look_at;
            cam_dir = glm::normalize(cam_dir);
            glm::vec3 horizontal = glm::normalize(glm::cross(cam_up, cam_dir));
            cam_pos = cam_pos + horizontal;
            cam_look_at = cam_look_at + horizontal;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        if (key == GLFW_KEY_S)
        {
            glm::vec3 cam_dir = - cam_pos + cam_look_at;
            cam_dir = glm::normalize(cam_dir);
            cam_pos = cam_pos - cam_dir;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        if (key == GLFW_KEY_D)
        {
            glm::vec3 cam_dir = - cam_pos + cam_look_at;
            cam_dir = glm::normalize(cam_dir);
            glm::vec3 horizontal = glm::normalize(glm::cross(cam_up, cam_dir));
            cam_pos = cam_pos - horizontal;
            cam_look_at = cam_look_at - horizontal;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        
        
        if (key == GLFW_KEY_Q)
        {
            cam_pos = cam_pos + cam_up;
            cam_look_at = cam_look_at + cam_up;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        
        
        if (key == GLFW_KEY_E)
        {
            cam_pos = cam_pos - cam_up;
            cam_look_at = cam_look_at - cam_up;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        if (key == GLFW_KEY_Y)
        {
            if(isSphere == 0){
                isSphere = 1;
            }else{
                isSphere = 0;
            }
            
        }
        if (key == GLFW_KEY_1){
            if (testTerrain->drawOutline == false)testTerrain->drawOutline = true;
            else testTerrain->drawOutline = false;
            
        }
        
	}
    
}

// Function transfer 2D point into 3D point
glm::vec3 trackBallMapping(double x, double y)
{
    
    glm::vec3 v;
    float d;
    v.x = (2.0 * x - origWidth) / origWidth;
    v.y = (origHeight - 2.0 * y) / origHeight;
    v.z = 0.0;
    d = glm::length(v);
    d = (d<1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d*d);
    v = glm::normalize(v);// Still need to normalize, since we only capped d, not v.
    return v;
    
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse == 1){
        glfwGetCursorPos(window, &oldX, &oldY);
        firstMouse = 0;
    }
    
    if(mouseClick == 1){
        //std::cout<< "OldX: " << oldX << " OldY: " << oldY << std::endl;
        
        // Transfer the 2-D start and end point in window into 3-D vec3 objects in world
        glm::vec3 lastPt = trackBallMapping(oldX, oldY);
        
        //std::cout << "Window width: " << origWidth << std::endl;
        //std::cout << "Window height: " << origHeight << std::endl;
        
        
        glm::vec3 curPt = trackBallMapping(xpos, ypos);
        
        if (glm::length(lastPt - curPt) > 0.001f){
            glm::vec3 rotAxis;
            rotAxis = glm::cross(lastPt, curPt);
            float cosTheta = dot(lastPt, curPt);
            float angle = acos(cosTheta / ((glm::length(lastPt) * glm::length(curPt))));
            
            glm::mat4 RMatrix = glm::rotate(glm::mat4(1.0f),angle, rotAxis);
            
            // Rotate object
//            squirtle->toWorld = RMatrix * squirtle->toWorld;
//            bulbasaur->toWorld = RMatrix * bulbasaur->toWorld;
//
//            squirtleSphere->toWorld = RMatrix * squirtleSphere->toWorld;
//            bulbasaurSphere->toWorld = RMatrix * bulbasaurSphere->toWorld;
            
            // Rotate camera
//            glm::vec3 direction = glm::normalize(cam_pos - cam_look_at);
//
//            glm::vec4 temp = glm::vec4(direction, 0.0f) * RMatrix;
//            cam_look_at = glm::vec3(cam_pos.x-temp.x, cam_pos.y-temp.y, cam_pos.z-temp.z);
            glm::vec3 cam_dir = -cam_pos + cam_look_at;
            cam_dir = glm::vec3(RMatrix * glm::vec4(cam_dir, 1));
            cam_look_at = cam_pos + cam_dir;

            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
    }
    oldX = xpos;
    oldY = ypos;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        mouseClick = 1;
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        mouseClick = 0;
    }

    
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
    //std::cout << "Offset: " << yoffset << std::endl;
    
    // change the scaler as we scroll our mouse in y direction
    float scaler = max(1.0 + yoffset * 0.01, 0.00001);
    
    //std::cout << "Scaler: " << scaler << std::endl;
    
    // scale the toWorld matrix according to our scaler
     squirtle->toWorld = glm::scale(squirtle->toWorld, glm::vec3(scaler, scaler, scaler));

}

