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


        float positions[] = 
        {
            0.0f, 0.0f, 0.0f, 0.0f, // 0, Bottom left
            540.0f, 0.0f, 1.0f, 0.0f, // 1, Bottom right
            540.0f, 540.0f, 1.0f, 1.0f, // 2, Top right
            0.0f, 540.0f, 0.0f, 1.0f, // 3, Top left
        };

        unsigned int indices[] = 
        {
             0,1,2, // First triangle
             2,3,0, // Second triangle
        };

        // Buffers & Layouts
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2); // Positions
        layout.Push<float>(2); // Vertex Coords
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);



        // Projection Matrix
        glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));

        glm::vec3 translation = glm::vec3(0, 0, 0);


        // Shaders
        Shader shader("res/shaders/Tex.shader");
        shader.Bind();

        // Textures
        Texture texture("res/textures/kitty.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        // Unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        // Initialize ImGui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
        ImGui::StyleColorsDark();

        // Main Loop
        while (!glfwWindowShouldClose(window))
        {
            // Clear Screen
            renderer.Clear();

            // ImGui New Frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Model View Projection Matrix
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;

            // Draw Tex
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
            

            // ImGui Test
            {
                ImGui::SliderFloat("Translation X", &translation.x, 0.0f, 1380.0f);
                ImGui::SliderFloat("Translation Y", &translation.y, 0.0f, 540.0f);
                ImGui::SliderFloat("Translation Z", &translation.z, -1.001f, 1.001f);

                ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
            }

            // ImGui Render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            
            // Swap Buffers and Poll Events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
	}
    // Close Window at End of Program
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}