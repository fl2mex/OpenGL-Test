#include "TestTex2D.h"
#include "Renderer.h"
#include "ImGui/imgui.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

namespace test
{
    TestTex2D::TestTex2D()
        : m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
          m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0)
	{
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
        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2); // Positions
        layout.Push<float>(2); // Vertex Coords
        m_VAO->AddBuffer(*m_VBO, layout);
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);

        // Translations
        glm::vec3 translationA = glm::vec3(0, 0, 0);
        glm::vec3 translationB = glm::vec3(0, 0, 0);

        // Shader and Texture
        m_Shader = std::make_unique<Shader>("res/shaders/Tex.shader");
        m_Shader->Bind();
        m_Texture = std::make_unique<Texture>("res/textures/kitty.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTex2D::~TestTex2D()
	{
	}
	void TestTex2D::OnUpdate(float deltaTime)
	{
	}

	void TestTex2D::OnRender()
	{
        // Clear Screen
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        {   // Scope for seperate Model View Projection Matrices
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
	}

	void TestTex2D::OnImGuiRender()
	{   // Slider for first Texture           Axis               Min      Max 
        ImGui::SliderFloat("Translation A-X", &m_TranslationA.x, 0.0f,    1380.0f);
        ImGui::SliderFloat("Translation A-Y", &m_TranslationA.y, 0.0f,    540.0f);
        ImGui::SliderFloat("Translation A-Z", &m_TranslationA.z, -1.001f, 1.001f);

        // Slider for second Texture          Axis               Min      Max 
        ImGui::SliderFloat("Translation B-X", &m_TranslationB.x, 0.0f,    1380.0f);
        ImGui::SliderFloat("Translation B-Y", &m_TranslationB.y, 0.0f,    540.0f);
        ImGui::SliderFloat("Translation B-Z", &m_TranslationB.z, -1.001f, 1.001f);

        // Display FPS
        ImGui::Separator();
        ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	}
}