#include "VoxymoreCore.hpp"
#include <iostream>

class Sandbox : public Voxymore::Core::Application{
public:
    Sandbox(){}
    ~Sandbox(){}
};

Voxymore::Core::Application* Voxymore::Core::CreateApplication() {
    VXM_INFO("Create Application");
    return new Sandbox();
}