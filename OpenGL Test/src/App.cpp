#include <GL/glew.h> // GLEW
#include <GLFW/glfw3.h> // GLFW

#include <iostream>

#include "Shader.h"

// ImGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

bool isWireframe = false;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

    // Create window, 1920x1080
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "App", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    // glfwSwapInterval(1);

    /* Initialize GLEW */ if (glewInit())
    {
        std::cout << "Error!" << std::endl;
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    Shader shader("res/shader/vertex.glsl", "res/shader/fragment.glsl");

    float vertices[] = {
     // Positions           Colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
    };
    unsigned int indices[] = {
        0, 1, 2,  // First triangle
    };

    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    ImGui::StyleColorsDark();


    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Render
        glClearColor(0.2f, 0.3f, 0.8f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (isWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // ImGui New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        shader.Bind();


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // ImGui Test
        ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

        // ImGui Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap Buffers and Poll Events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
        return 0; 
    }
}