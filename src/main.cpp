#include <iostream>

#include "VoxymoreCore.hpp"

class Sandbox : public Voxymore::Core::Application{
public:
    Sandbox(){

    }
    ~Sandbox(){

    }
};

int main() {
//    Voxymore::Core::hello();
//    return 0;
    Sandbox* sandbox = new Sandbox();
    sandbox->Run();
    delete sandbox;
}
