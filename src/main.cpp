#include "Voxymore/Voxymore.hpp"
#include <iostream>

class Sandbox : public Voxymore::Core::Application{
public:
    Sandbox(){
        PushOverlay(new Voxymore::Core::ImGUILayer());
    }
    ~Sandbox(){}
};

Voxymore::Core::Application* Voxymore::Core::CreateApplication() {
    VXM_INFO("Create Application");
    return new Sandbox();
}