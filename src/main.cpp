#define NEWLINE "\n"

#include "Voxymore/Voxymore.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/PerspectiveCameraController.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "../lib/VoxymoreCore/platform/OpenGL/Voxymore/OpenGL/OpenGLShader.hpp"
#include <iostream>
#include <imgui.h>

class ExampleLayer : public Voxymore::Core::Layer {
private:
    Voxymore::Core::Ref<Voxymore::Core::Shader> m_Shader;
    Voxymore::Core::Ref<Voxymore::Core::Shader> m_TextureShader;
    Voxymore::Core::Ref<Voxymore::Core::Texture2D> m_Texture;

    Voxymore::Core::Ref<Voxymore::Core::VertexArray> m_VertexArray;
    Voxymore::Core::Ref<Voxymore::Core::VertexBuffer> m_VertexBuffer;
    Voxymore::Core::Ref<Voxymore::Core::IndexBuffer> m_IndexBuffer;

    Voxymore::Core::Ref<Voxymore::Core::VertexArray> m_SquareVertexArray;
    Voxymore::Core::Ref<Voxymore::Core::VertexBuffer> m_SquareVertexBuffer;
    Voxymore::Core::Ref<Voxymore::Core::IndexBuffer> m_SquareIndexBuffer;

    Voxymore::Core::PerspectiveCameraController m_Camera;
private:
    glm::vec3 modelPos = {0,0,-2};
    glm::vec3 modelRot = {0,0,0};
    glm::vec3 modelScale = {1,1,1};
public:
    ExampleLayer() : Voxymore::Core::Layer("ExampleLayer"), m_Camera(Voxymore::Core::Application::Get().GetWindow().GetWidth(), Voxymore::Core::Application::Get().GetWindow().GetHeight())
    {
        Voxymore::Core::Application::Get().GetWindow().SetCursorState(m_Camera.GetEnable() ? Voxymore::Core::CursorState::Locked : Voxymore::Core::CursorState::None);
        const Voxymore::Core::Window& window = Voxymore::Core::Application::Get().GetWindow();


        m_VertexArray.reset(Voxymore::Core::VertexArray::Create());

        float cubeVertices [(8 * 3) + (8 * 4)] = {
                -0.5f, -0.5f, +0.5f,   0.8f, 0.1f, 0.7f, 1.0f,
                +0.5f, -0.5f, +0.5f,   0.1f, 0.7f, 0.8f, 1.0f,
                +0.5f, +0.5f, +0.5f,   0.8f, 0.8f, 0.8f, 1.0f,
                -0.5f, +0.5f, +0.5f,   0.1f, 0.1f, 0.1f, 1.0f,

                -0.5f, -0.5f, -0.5f,   0.8, 0.2f, 0.3f, 1.0f,
                +0.5f, -0.5f, -0.5f,   0.2f, 0.3f, 0.8f, 1.0f,
                +0.5f, +0.5f, -0.5f,   0.3f, 0.8f, 0.2f, 1.0f,
                -0.5f, +0.5f, -0.5f,   0.7f, 0.8f, 0.1f, 1.0f,
        };
        m_VertexBuffer.reset(Voxymore::Core::VertexBuffer::Create(sizeof(cubeVertices), cubeVertices));
        Voxymore::Core::BufferLayout layout = {
                {Voxymore::Core::ShaderDataType::Float3, "a_Position"},
                {Voxymore::Core::ShaderDataType::Float4, "a_Color"},
        };
        m_VertexBuffer->SetLayout(layout);


        m_VertexArray->AddVertexBuffer(m_VertexBuffer);


        uint32_t cubeIndices[2 * 3 * 6] = {
                0,1,2,
                0,2,3,
                1,5,6,
                1,6,2,
                5,4,7,
                5,7,6,
                4,0,3,
                4,3,7,
                4,5,1,
                4,1,0,
                3,2,6,
                3,6,7,
        };

        m_IndexBuffer.reset(Voxymore::Core::IndexBuffer::Create(std::size(cubeIndices), cubeIndices));

        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        m_SquareVertexArray.reset(Voxymore::Core::VertexArray::Create());
        float squareVertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };
        m_SquareVertexBuffer.reset(Voxymore::Core::VertexBuffer::Create(sizeof(squareVertices), squareVertices));
        Voxymore::Core::BufferLayout squareLayout = {
                {Voxymore::Core::ShaderDataType::Float3, "a_Position"},
                {Voxymore::Core::ShaderDataType::Float2, "a_TexCoord"},
        };
        m_SquareVertexBuffer->SetLayout(squareLayout);
        m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        m_SquareIndexBuffer.reset(Voxymore::Core::IndexBuffer::Create(std::size(squareIndices), squareIndices));
        m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

        VXM_INFO("Creat FlatColor Shader");
        m_Shader = Voxymore::Core::Shader::Create("assets/shaders/FlatColor.glsl");
        VXM_INFO("Creat Texture Shader");
        m_TextureShader = Voxymore::Core::Shader::Create("assets/shaders/TextureShader.glsl");

        m_Texture = Voxymore::Core::Texture2D::Create("assets/textures/texture_checker.png");
        std::dynamic_pointer_cast<Voxymore::Core::OpenGLShader>(m_TextureShader)->Bind();
        std::dynamic_pointer_cast<Voxymore::Core::OpenGLShader>(m_TextureShader)->SetUniformInt("u_Texture", 0);
        std::dynamic_pointer_cast<Voxymore::Core::OpenGLShader>(m_TextureShader)->Unbind();
    }


    bool UpdateCameraPositionPressed(Voxymore::Core::KeyPressedEvent& event) {
        if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_ESCAPE && event.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY ESCAPE.");
            m_Camera.SetEnable(!m_Camera.GetEnable());
            Voxymore::Core::Application::Get().GetWindow().SetCursorState(m_Camera.GetEnable() ? Voxymore::Core::CursorState::Locked : Voxymore::Core::CursorState::None);
        }
        return false;
    }

    virtual void OnUpdate(Voxymore::Core::TimeStep timeStep) override {
        m_Camera.OnUpdate(timeStep);


        Voxymore::Core::Renderer::BeginScene(m_Camera.GetCamera());

        m_Texture->Bind();
        Voxymore::Core::Renderer::Submit(m_TextureShader, m_SquareVertexArray);
        Voxymore::Core::Renderer::Submit(m_Shader, m_VertexArray, Voxymore::Core::Math::TRS(modelPos, glm::quat(glm::radians(modelRot)), modelScale));

        Voxymore::Core::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {
//        ImGui::Begin("Camera Component");
//
//        glm::vec3 position = m_Camera.GetPosition();
//        ImGui::DragFloat3("Camera Position", glm::value_ptr(position));
//        if(position != m_Camera.GetPosition()) {
//            m_Camera.SetPosition(position);
//        }
//
//        glm::quat rotation = m_Camera.GetRotation();
//        glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
//        ImGui::DragFloat3("Camera Rotation", glm::value_ptr(euler));
//        if(euler != glm::degrees(glm::eulerAngles(m_Camera.GetRotation()))) {
//            m_Camera.SetRotation(glm::quat(glm::radians(euler)));
//        }
//
//        float fov = m_Camera.GetFOV();
//        ImGui::SliderFloat("fov", &fov, 45.0f, 110.0f);
//        if(fov != m_Camera.GetFOV()){
//            m_Camera.SetFOV(fov);
//        }
//
//        ImGui::SliderFloat("sensitivity", &sensitivity, 0.01f, 2.0f);
//        ImGui::DragFloat("speed", &speed, 0.01f);
//
//        bool vsync = Voxymore::Core::Application::Get().GetWindow().IsVSync();
//        ImGui::Checkbox("VSync", &vsync);
//        Voxymore::Core::Application::Get().GetWindow().SetVSync(vsync);
//        ImGui::End();

        ImGui::Begin("Model Component");

        ImGui::DragFloat3("Position", glm::value_ptr(modelPos));
        ImGui::DragFloat3("Rotation", glm::value_ptr(modelRot));
        ImGui::DragFloat3("Scale", glm::value_ptr(modelScale));

        ImGui::End();
    }

    virtual void OnEvent(Voxymore::Core::Event& event) {
        m_Camera.OnEvent(event);

        Voxymore::Core::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Voxymore::Core::KeyPressedEvent>(BIND_EVENT_FN(ExampleLayer::UpdateCameraPositionPressed, std::placeholders::_1));
    }
};

class Sandbox : public Voxymore::Core::Application{
public:
    inline Sandbox() : Voxymore::Core::Application() {
        PushLayer(new ExampleLayer());
    }

    inline ~Sandbox(){}
};

Voxymore::Core::Application* Voxymore::Core::CreateApplication() {
    VXM_INFO("Create Application");
    return new Sandbox();
}