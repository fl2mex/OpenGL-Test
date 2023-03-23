#include <GL/glew.h> // GLEW
#include <GLFW/glfw3.h> // GLFW
#include <iostream> // std library

// Abstracted OpenGL calls
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "GLM/glm.hpp" // GLM
#include "GLM/gtc/matrix_transform.hpp" //GLM Matrices

// ImGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

// Tests
#include "Tests/TestClearColor.h"
#include "Tests/TestTex2D.h"

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) // Initialize GLFW
        return -1;

    // Set OpenGL to 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window, 1920x1080
    window = glfwCreateWindow(1920, 1080, "App", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // V-Sync on
    glfwSwapInterval(1);

    // Initialize GLEW, check for errors, print OpenGL version
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    else
        std::cout << glGetString(GL_VERSION) << std::endl;


    // Scope bug fix for persistent terminal
    {
        // Enable Alpha Blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        // Initialize ImGui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
        ImGui::StyleColorsDark();

        // Test Menu
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTex2D>("2D Texture");

        // Main Loop
        while (!glfwWindowShouldClose(window))
        {
            // Clear Screen
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            // ImGui New Frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Test Menu
            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            // ImGui Render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap Buffers and Poll Events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // Delete Test Menu when Main Loop ends
        delete currentTest;
        if (currentTest != testMenu)
			delete testMenu;
	}
    // Close Window at End of Program
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}