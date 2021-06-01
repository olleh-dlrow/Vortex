#include <Vortex.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
tips:
To copy the dll automatically, you can 
add below command to sandbox build events, pre-build event:
xcopy "$(SolutionDir)bin\$(Configuration)-$(Platform)\Vortex\Vortex.dll" "$(SolutionDir)bin\$(Configuration)-$(Platform)\Sandbox" /y
*/

class ExampleLayer: public Vortex::Layer {
public:
    ExampleLayer(): 
        Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        m_VertexArray.reset(Vortex::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        Vortex::Ref<Vortex::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Vortex::VertexBuffer::Create(vertices, sizeof(vertices)));
        Vortex::BufferLayout layout = {
            { Vortex::ShaderDataType::Float3, "a_Position" },
            { Vortex::ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        Vortex::Ref<Vortex::IndexBuffer> indexBuffer;
        indexBuffer.reset(Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_SquareVA.reset(Vortex::VertexArray::Create());

        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Vortex::Ref<Vortex::VertexBuffer> squareVB;
        squareVB.reset(Vortex::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { Vortex::ShaderDataType::Float3, "a_Position" },
            { Vortex::ShaderDataType::Float2, "a_TexCoord" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Vortex::Ref<Vortex::IndexBuffer> squareIB;
        squareIB.reset(Vortex::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;
            out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
                v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

        std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
            in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
			}
		)";

        m_Shader.reset(Vortex::Shader::Create(vertexSrc, fragmentSrc));

        std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

        std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

            uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

        m_FlatColorShader.reset(Vortex::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

        std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec2 v_TexCoord;
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

        std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

        m_TextureShader.reset(Vortex::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

        m_Texture = Vortex::Texture2D::Create("assets/textures/Checkerboard.png");
        m_ChernoLogoTexture = Vortex::Texture2D::Create("assets/textures/ChernoLogo.png");

        std::dynamic_pointer_cast<Vortex::OpenGLShader>(m_TextureShader)->Bind();
        std::dynamic_pointer_cast<Vortex::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Vortex::Timestep ts) override
    {
        // handle input
        if (Vortex::Input::IsKeyPressed(VT_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Vortex::Input::IsKeyPressed(VT_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;

        if (Vortex::Input::IsKeyPressed(VT_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        else if (Vortex::Input::IsKeyPressed(VT_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        if (Vortex::Input::IsKeyPressed(VT_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (Vortex::Input::IsKeyPressed(VT_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        Vortex::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Vortex::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        // render
        Vortex::Renderer::BeginScene(m_Camera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        
        // question: we still need to convert Shader to OpenGLShder, could this make packing shader no meaning?
        std::dynamic_pointer_cast<Vortex::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<Vortex::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Vortex::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }

        m_Texture->Bind();
        Vortex::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        m_ChernoLogoTexture->Bind();
        Vortex::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        // Triangle
        // Vortex::Renderer::Submit(m_Shader, m_VertexArray);

        Vortex::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void OnEvent(Vortex::Event& event) override
    {

    }

private:
    Vortex::Ref<Vortex::Shader> m_Shader;
    Vortex::Ref<Vortex::VertexArray> m_VertexArray;

    Vortex::Ref<Vortex::Shader> m_FlatColorShader, m_TextureShader;
    Vortex::Ref<Vortex::VertexArray> m_SquareVA;

    Vortex::Ref<Vortex::Texture2D> m_Texture, m_ChernoLogoTexture;

    Vortex::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 5.0f;

    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 180.0f;

    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox: public Vortex::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }
    ~Sandbox() {
    
    }
};

Vortex::Application* Vortex::CreateApplication() {
    VT_INFO("Create application");
    return new Sandbox();
}