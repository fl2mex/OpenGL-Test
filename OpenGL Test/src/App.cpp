#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "App", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // V-Sync on
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    else
        std::cout << glGetString(GL_VERSION) << std::endl;

    // Scope bug fix for persistent terminal
    {

        float positions[] = 
        {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f, // 3
        };

        unsigned int indices[] = 
        {
             0,1,2, // First triangle
             2,3,0, // Second triangle
        };

        // Enable Alpha Blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Buffers & Layouts
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2); // Positions
        layout.Push<float>(2); // Vertex Coords

        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        // Projection Matrix
        glm::mat4 proj = glm::ortho((-16.0f/9.0f)/2.0f, (16.0f / 9.0f)/2.0f, -1.0f/2.0f, 1.0f/2.0f, -1.0f, 1.0f);

        // Shaders
        Shader shader("res/shaders/Tex.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.2f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

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


        // Color transforms
        float r = 0.0f;
        float increment = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            // Draw triangles
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);
            
            // Transform Colors
            if (r > 1.0f)
			    increment = -0.05f;
		    else if (r < 0.0f)
			    increment = 0.05f;
            r += increment;
            
            // Swap Buffers and Poll Events?
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
	}
    // Close Window at End of Program
    glfwTerminate();
    return 0;
}