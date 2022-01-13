#ifndef SIMPLEPHYSICSENGINE_MAIN_H
#define SIMPLEPHYSICSENGINE_MAIN_H
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

#include <thread>
#include <iostream>
#include <vector>
#include "glad/glad.h"		
#include "GLFW/glfw3.h" 
#include "PhysicsEngine.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Objects.h"
#include <list>


//GLFW
void ProcessInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
//callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//GLFW

//PhysicsEngine
void AddSphereToPhysicsWorld(utils::Vector3 position = utils::Vector3{ 0,0,0 }, utils::Vector3 rotation = utils::Vector3{ 0,0,0 }, utils::Vector3 velocity = utils::Vector3{ 0,0,0 }, utils::Vector3 forces = utils::Vector3{ 0,0,0 }, float mass = 0, bool usePhysics = true, float radius = 1.0f, int sectorCount = 36, int stackCount = 18, Shader* shader = 0);
utils::Vector3 zero(0, 0, 0);

const char* vertexShaderPath = "vertexShader.vs";
const char* fragmentShaderPath = "fragmentShader.fs";
const char* lampFragmentShaderPath = "lampFragmentShader.fs";

using namespace std;

#endif //SIMPLEPHYSICSENGINE_MAIN_H
