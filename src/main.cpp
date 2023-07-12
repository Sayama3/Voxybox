#include "VoxymoreCore.hpp"
#include <iostream>

class Sandbox : public Voxymore::Core::Application{
public:
    Sandbox(){}
    ~Sandbox(){}
};

Voxymore::Core::Application* Voxymore::Core::CreateApplication() {
    std::cout << "Create Application" << std::endl;
    return new Sandbox();
}