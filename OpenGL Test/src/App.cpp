#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1080, 1080, "App", NULL, NULL);
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
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f, // 3
        };

        unsigned int indices[] = 
        {
             0,1,2, // First triangle
             2,3,0, // Second triangle
        };


        // Buffers & Layouts
        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        // Shaders
        Shader shader("res/shaders/Triangle.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // Color transforms
        float r = 0.0f;
        float increment = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
            // Draw triangles
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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