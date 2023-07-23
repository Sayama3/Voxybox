#include "Voxymore/Voxymore.hpp"
#include "Voxymore/Macros.hpp"
#include <iostream>
#include <imgui.h>

class ExampleLayer : public Voxymore::Core::Layer {
private:
	std::shared_ptr<Voxymore::Core::Shader> m_Shader;
	std::shared_ptr<Voxymore::Core::VertexArray> m_VertexArray;
	std::shared_ptr<Voxymore::Core::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Voxymore::Core::IndexBuffer> m_IndexBuffer;

	Voxymore::Core::PerspectiveCamera m_Camera;
public:
    ExampleLayer() : Voxymore::Core::Layer("ExampleLayer"), m_Camera(Voxymore::Core::Application::Get().GetWindow().GetWidth(), Voxymore::Core::Application::Get().GetWindow().GetHeight(), 60.0f){
		const Voxymore::Core::Window& window = Voxymore::Core::Application::Get().GetWindow();
		m_Camera.SetSize(window.GetWidth(), window.GetHeight());

		m_VertexArray.reset(Voxymore::Core::VertexArray::Create());

		float vertices [(3 * 3) + (3 * 3) + (3 * 4)] = {
				-0.5f, -0.5f, 0.0f,    -0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, -0.5f, 1.0f,
				0.0f, 0.5f, 0.0f,       0.0f, 0.5f, 0.0f,       0.0f, 0.5f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.0f,      0.5f, -0.5f, 0.5f,      0.5f, -0.5f, 0.5f, 1.0f,
		};
		m_VertexBuffer.reset(Voxymore::Core::VertexBuffer::Create(sizeof(vertices), vertices));
		Voxymore::Core::BufferLayout layout = {
				{Voxymore::Core::ShaderDataType::Float3, "a_Position"},
				{Voxymore::Core::ShaderDataType::Float3, "a_Normal"},
				{Voxymore::Core::ShaderDataType::Float4, "a_Color"},
		};
		m_VertexBuffer->SetLayout(layout);


		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t index[3] = {
				0,
				2,
				1,
		};

		m_IndexBuffer.reset(Voxymore::Core::IndexBuffer::Create(std::size(index), index));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec3 a_Normal;
            layout(location = 2) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

            out vec3 v_Position;
            out vec4 v_Color;

            void main() {
                gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
                v_Position = (u_ViewProjectionMatrix * vec4(a_Position, 1.0)).xyz;
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
		m_Shader.reset(Voxymore::Core::Shader::CreateShader({vertexSrc, Voxymore::Core::ShaderType::VERTEX_SHADER}, {fragmentSrc, Voxymore::Core::ShaderType::FRAGMENT_SHADER}));

	}

	bool UpdateCameraSize(Voxymore::Core::WindowResizeEvent& event) {
		m_Camera.SetSize(event.GetWidth(), event.GetHeight());
		return false;
	}

	virtual void OnUpdate() override {
            Voxymore::Core::Renderer::BeginScene(m_Camera);

            Voxymore::Core::Renderer::Submit(m_Shader, m_VertexArray);

            Voxymore::Core::Renderer::EndScene();
	}

    virtual void OnImGuiRender() override {

        ImGui::Begin("Camera Component");

		glm::vec3 position = m_Camera.GetPosition();
		ImGui::InputFloat3("Camera Position", glm::value_ptr(position));
		if(position != m_Camera.GetPosition()) {
			m_Camera.SetPosition(position);
		}

		glm::quat rotation = m_Camera.GetRotation();
		glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
		ImGui::InputFloat3("Camera Rotation", glm::value_ptr(euler));
		if(euler != glm::degrees(glm::eulerAngles(m_Camera.GetRotation()))) {
			m_Camera.SetRotation(glm::quat(glm::radians(euler)));
		}

		float fov = m_Camera.GetFOV();
		ImGui::SliderFloat("fov", &fov, 45.0f, 110.0f);
		if(fov != m_Camera.GetFOV()){
			m_Camera.SetFOV(fov);
		}


        ImGui::End();
    }

	virtual void OnEvent(Voxymore::Core::Event& event) {
		Voxymore::Core::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Voxymore::Core::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::UpdateCameraSize, std::placeholders::_1));
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