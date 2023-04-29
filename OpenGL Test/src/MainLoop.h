#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

// Screen
const unsigned int screenWidth = 1920;
const unsigned int screenHeight = 1080;
bool isWireframe = false;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		std::cout << "Escape Key pressed, Exiting program..." << std::endl;
	}
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		isWireframe = !isWireframe;
		std::cout << "Wireframe Toggled" << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

class App
{
public:
	App() // Constructor, no need for init call
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL Version to 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(screenWidth, screenHeight, "App", NULL, NULL);
		glfwMakeContextCurrent(window);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set Blending Function
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		glfwSwapInterval(1); // Enable VSync
		glewInit();

		// Callbacks
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		ImGui::CreateContext(); // Create ImGui Context
		ImGui_ImplGlfw_InitForOpenGL(window, true); // Initialize ImGui
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
		ImGui::StyleColorsDark(); // Set ImGui Style to Dark
	}
	~App() // Destructor, no need for cleanup call
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}
	void Run()
	{
		Shader ourShader("res/shader/vertex.glsl", "res/shader/fragment.glsl");
		Model ourModel("res/model/backpack.obj");

		while (!glfwWindowShouldClose(window)) // Main Loop
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Render
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ourShader.Bind();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ourShader.SetUniformMat4f("projection", projection);
			ourShader.SetUniformMat4f("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			ourShader.SetUniformMat4f("model", model);
			ourModel.Draw(ourShader);

			// ImGui Test
			ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Swap Buffers and Poll Events
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
private:
	GLFWwindow* window;
};