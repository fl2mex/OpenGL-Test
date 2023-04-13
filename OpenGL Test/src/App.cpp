#include <GL/glew.h> // GLEW
#include <GLFW/glfw3.h> // GLFW
#include <iostream>

#include "Shader.h" // Shader Abstraction
#include "Texture.h" // Texture Abstraction
#include "Camera.h" // Camera Abstraction

#include "GLM/glm.hpp" // GLM
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ImGui/imgui_impl_glfw.h" // ImGui
#include "ImGui/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h" // stb_image

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

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

int main(void)
{
    /* Initialize GLFW */ if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
    }
    /* Set OpenGL to 3.3 Core */ {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    }
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "App", NULL, NULL);
    /* Check if window created */ if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* GLFW Settings */{
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // Viewport Updates
        glfwSetKeyCallback(window, keyCallback); // Key Callback
        glfwSwapInterval(1); // Enable VSync
        glEnable(GL_DEPTH_TEST); // Enable Z-Buffer
        glfwSetCursorPosCallback(window, mouseCallback); // Mouse Callback
        glfwSetScrollCallback(window, scrollCallback); // Mouse Scroll Callback
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide Cursor
    }
    /* Initialize GLEW */ if (glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
    unsigned int VBO, cubeVAO, lightCubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(lightCubeVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // Initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    ImGui::StyleColorsDark();

    Shader lightingShader("res/shader/vertex.glsl", "res/shader/fragment.glsl");
    Shader lightCubeShader("res/shader/cubevertex.glsl", "res/shader/cubefragment.glsl");
    Texture diffuseMap("res/texture/container.png");
    Texture specularMap("res/texture/container_specular.png");

    lightingShader.Bind();
    lightingShader.SetUniform1i("material.diffuse", 0);
    lightingShader.SetUniform1i("material.specular", 1);


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);

        // ImGui New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        lightingShader.Bind();
        lightingShader.SetUniformVec3f("viewPos", camera.Position);
        lightingShader.SetUniform1f("material.shininess", 32.0f);

        lightingShader.SetUniformVec3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        lightingShader.SetUniformVec3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniformVec3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.SetUniformVec3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        // point light 1
        lightingShader.SetUniformVec3f("pointLights[0].position", pointLightPositions[0]);
        lightingShader.SetUniformVec3f("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniformVec3f("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniformVec3f("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[0].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[0].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightingShader.SetUniformVec3f("pointLights[1].position", pointLightPositions[1]);
        lightingShader.SetUniformVec3f("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniformVec3f("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniformVec3f("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[1].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[1].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightingShader.SetUniformVec3f("pointLights[2].position", pointLightPositions[2]);
        lightingShader.SetUniformVec3f("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniformVec3f("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniformVec3f("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[2].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[2].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightingShader.SetUniformVec3f("pointLights[3].position", pointLightPositions[3]);
        lightingShader.SetUniformVec3f("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniformVec3f("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniformVec3f("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[3].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[3].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[3].quadratic", 0.032f);
        // spotLight
        lightingShader.SetUniformVec3f("spotLight.position", camera.Position);
        lightingShader.SetUniformVec3f("spotLight.direction", camera.Front);
        lightingShader.SetUniformVec3f("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.SetUniformVec3f("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniformVec3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("spotLight.constant", 1.0f);
        lightingShader.SetUniform1f("spotLight.linear", 0.09f);
        lightingShader.SetUniform1f("spotLight.quadratic", 0.032f);
        lightingShader.SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.SetUniformMat4f("projection", projection);
        lightingShader.SetUniformMat4f("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.SetUniformMat4f("model", model);
        
        diffuseMap.Bind(0);
        specularMap.Bind(1);

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.SetUniformMat4f("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightCubeShader.Bind();
        lightCubeShader.SetUniformMat4f("projection", projection);
        lightCubeShader.SetUniformMat4f("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.SetUniformMat4f("model", model);
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.SetUniformMat4f("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ImGui Test
        ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

        // ImGui Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap Buffers and Poll Events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /* Cleanup */{
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteBuffers(1, &VBO);

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
        return 0; 
    }
}