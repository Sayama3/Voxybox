#include "Voxymore/Voxymore.hpp"
#include "Voxymore/Macros.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include <iostream>
#include <imgui.h>

class ExampleLayer : public Voxymore::Core::Layer {
private:
    std::shared_ptr<Voxymore::Core::Shader> m_Shader;
    std::shared_ptr<Voxymore::Core::VertexArray> m_VertexArray;
    std::shared_ptr<Voxymore::Core::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Voxymore::Core::IndexBuffer> m_IndexBuffer;

    Voxymore::Core::PerspectiveCamera m_Camera;
private:
    bool updateCamera = true;
    bool hasSetMouse = false;
    float mouseX, mouseY = -1;
    float sensitivity = 1.0f;
    float speed = 1.0f;
    glm::vec3 movement = {0,0,0};
    glm::vec3 modelPos = {0,0,-1};
    glm::vec3 modelRot = {0,0,0};
    glm::vec3 modelScale = {1,1,1};
public:
    ExampleLayer() : Voxymore::Core::Layer("ExampleLayer"), m_Camera(Voxymore::Core::Application::Get().GetWindow().GetWidth(), Voxymore::Core::Application::Get().GetWindow().GetHeight(), 60.0f)
    {
        Voxymore::Core::Application::Get().GetWindow().SetCursorState(updateCamera ? Voxymore::Core::CursorState::Locked : Voxymore::Core::CursorState::None);
        const Voxymore::Core::Window& window = Voxymore::Core::Application::Get().GetWindow();
        m_Camera.SetSize(window.GetWidth(), window.GetHeight());
        m_Camera.SetRotation(glm::quatLookAt(glm::vec3{0,0,-1}, glm::vec3{0,1,0}));
        m_Camera.UpdateAllMatrix();


        m_VertexArray.reset(Voxymore::Core::VertexArray::Create());

        float vertices [(3 * 3) + (3 * 4)] = {
                -0.5f, -0.5f, 0.0f,     0.8, 0.2f, 0.3f, 1.0f,
                0.5f, -0.5f, 0.0f,      0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f,       0.3f, 0.8f, 0.2f, 1.0f,
        };
        m_VertexBuffer.reset(Voxymore::Core::VertexBuffer::Create(sizeof(vertices), vertices));
        Voxymore::Core::BufferLayout layout = {
                {Voxymore::Core::ShaderDataType::Float3, "a_Position"},
                {Voxymore::Core::ShaderDataType::Float4, "a_Color"},
        };
        m_VertexBuffer->SetLayout(layout);


        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t index[3] = {
                0,
                1,
                2,
        };

        m_IndexBuffer.reset(Voxymore::Core::IndexBuffer::Create(std::size(index), index));

        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main() {
                gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
                v_Position = (u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0)).xyz;
                v_Color = a_Color;
            }
            )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 o_Color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main() {
                o_Color = v_Color;
            }
        )";
        m_Shader.reset(Voxymore::Core::Shader::Create(vertexSrc, fragmentSrc));
    }

    bool UpdateCameraSize(Voxymore::Core::WindowResizeEvent& event) {
        m_Camera.SetSize(event.GetWidth(), event.GetHeight());
        return false;
    }

    bool UpdateCameraPositionPressed(Voxymore::Core::KeyPressedEvent& event) {
        if(updateCamera) {
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_W && event.GetRepeatCount() == 0) {
                VXM_CORE_INFO("Press KEY W.");
                movement += glm::vec3(0, 0, -1);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_S && event.GetRepeatCount() == 0) {
                VXM_CORE_INFO("Press KEY S.");
                movement += glm::vec3(0, 0, 1);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_D && event.GetRepeatCount() == 0) {
                VXM_CORE_INFO("Press KEY D.");
                movement += glm::vec3(1, 0, 0);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_A && event.GetRepeatCount() == 0) {
                VXM_CORE_INFO("Press KEY A.");
                movement += glm::vec3(-1, 0, 0);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_E && event.GetRepeatCount() == 0) {
                VXM_CORE_INFO("Press KEY E.");
                movement += glm::vec3(0, 1, 0);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_Q && event.GetRepeatCount() == 0) {
                VXM_CORE_INFO("Press KEY Q.");
                movement += glm::vec3(0, -1, 0);
            }
        }

        if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_ESCAPE && event.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY ESCAPE.");
            updateCamera = !updateCamera;
            Voxymore::Core::Application::Get().GetWindow().SetCursorState(updateCamera ? Voxymore::Core::CursorState::Locked : Voxymore::Core::CursorState::None);
            movement = {0,0,0};
            hasSetMouse = false;
        }
        return false;
    }

    bool UpdateCameraPositionReleased(Voxymore::Core::KeyReleasedEvent& event) {
        if(updateCamera) {
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_W) {
                VXM_CORE_INFO("Release KEY W.");
                movement -= glm::vec3(0, 0, -1);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_S) {
                VXM_CORE_INFO("Release KEY S.");
                movement -= glm::vec3(0, 0, 1);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_D) {
                VXM_CORE_INFO("Release KEY D.");
                movement -= glm::vec3(1, 0, 0);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_A) {
                VXM_CORE_INFO("Release KEY A.");
                movement -= glm::vec3(-1, 0, 0);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_E) {
                VXM_CORE_INFO("Release KEY E.");
                movement -= glm::vec3(0, 1, 0);
            }
            if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_Q) {
                VXM_CORE_INFO("Release KEY Q.");
                movement -= glm::vec3(0, -1, 0);
            }
        }
        if (event.GetKeyCode() == Voxymore::Core::KeyCode::KEY_ESCAPE) {
            VXM_CORE_INFO("Release KEY ESCAPE.");
            movement = {0,0,0};
        }
        return false;
    }

    bool UpdateCameraRotation(Voxymore::Core::MouseMovedEvent& event) {
        if(updateCamera) {
            if (!hasSetMouse) {
                mouseX = event.GetX();
                mouseY = event.GetY();
                hasSetMouse = true;
                return false;
            }
            glm::vec2 currentPos = {event.GetX(), event.GetY()};
            glm::vec2 olPos = {mouseX, mouseY};

            glm::vec2 deltaMove = currentPos - olPos;

            // old pos.
            mouseX = event.GetX();
            mouseY = event.GetY();

            glm::quat rotation = m_Camera.GetRotation();
            VXM_INFO("Mouse Delta: x:{0}, y: {1}", deltaMove.x, deltaMove.y);
            glm::vec2 movement = glm::radians(glm::vec2(deltaMove.x, deltaMove.y)) * (sensitivity);
            glm::vec3 right = glm::vec3{1, 0, 0};
            glm::vec3 up = {0, 1, 0};
            auto upwardRotation = glm::rotate(glm::identity<glm::quat>(), -movement.y, right); // Inverse because that's the way it works.
            auto rightwardRotation = glm::rotate(glm::identity<glm::quat>(), -movement.x, up); // Inverse because that's the way it works.
            rotation = rotation * upwardRotation; // Around the local right axis of the current rotation.
            rotation = rightwardRotation * rotation; // Around the global up axis.
            m_Camera.SetRotation(rotation);
        }
        return false;
    }

    virtual void OnUpdate(Voxymore::Core::TimeStep timeStep) override {
        {
            glm::vec3 position = m_Camera.GetPosition();
            glm::quat rotation = m_Camera.GetRotation();
            if (glm::length2(movement) != 0) {
                position += glm::normalize(rotation * movement) * (speed * timeStep.GetSeconds());
                m_Camera.SetPosition(position);
            }
        }

        m_Camera.UpdateAllMatrix();
        Voxymore::Core::Renderer::BeginScene(m_Camera);

        Voxymore::Core::Renderer::Submit(m_Shader, m_VertexArray, Voxymore::Core::Math::TRS(modelPos, glm::quat(glm::radians(modelRot)), modelScale));

        Voxymore::Core::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {

        ImGui::Begin("Camera Component");

        glm::vec3 position = m_Camera.GetPosition();
        ImGui::DragFloat3("Camera Position", glm::value_ptr(position));
        if(position != m_Camera.GetPosition()) {
            m_Camera.SetPosition(position);
        }

        glm::quat rotation = m_Camera.GetRotation();
        glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
        ImGui::DragFloat3("Camera Rotation", glm::value_ptr(euler));
        if(euler != glm::degrees(glm::eulerAngles(m_Camera.GetRotation()))) {
            m_Camera.SetRotation(glm::quat(glm::radians(euler)));
        }

        float fov = m_Camera.GetFOV();
        ImGui::SliderFloat("fov", &fov, 45.0f, 110.0f);
        if(fov != m_Camera.GetFOV()){
            m_Camera.SetFOV(fov);
        }

        ImGui::SliderFloat("sensitivity", &sensitivity, 0.01f, 2.0f);
        ImGui::DragFloat("speed", &speed, 0.01f);

        bool vsync = Voxymore::Core::Application::Get().GetWindow().IsVSync();
        ImGui::Checkbox("VSync", &vsync);
        Voxymore::Core::Application::Get().GetWindow().SetVSync(vsync);
        ImGui::End();

        ImGui::Begin("Model Component");

        ImGui::DragFloat3("Position", glm::value_ptr(modelPos));
        ImGui::DragFloat3("Rotation", glm::value_ptr(modelRot));
        ImGui::DragFloat3("Scale", glm::value_ptr(modelScale));

        ImGui::End();
    }

    virtual void OnEvent(Voxymore::Core::Event& event) {
        Voxymore::Core::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Voxymore::Core::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::UpdateCameraSize, std::placeholders::_1));
        dispatcher.Dispatch<Voxymore::Core::KeyPressedEvent>(BIND_EVENT_FN(ExampleLayer::UpdateCameraPositionPressed, std::placeholders::_1));
        dispatcher.Dispatch<Voxymore::Core::KeyReleasedEvent>(BIND_EVENT_FN(ExampleLayer::UpdateCameraPositionReleased, std::placeholders::_1));
        dispatcher.Dispatch<Voxymore::Core::MouseMovedEvent>(BIND_EVENT_FN(ExampleLayer::UpdateCameraRotation, std::placeholders::_1));
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