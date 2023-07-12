cmake_minimum_required(VERSION 3.24)

# set enable the debugger (when the debug mode is enable?)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

set(VoxymoreCore_SHARED_BUILD ON)
add_subdirectory(lib/VoxymoreCore)

project(VoxyboxProject)

add_executable(Voxybox)

set(CMAKE_CXX_STANDARD 20)
target_compile_features(Voxybox PUBLIC cxx_std_20)

include_directories(include)
add_subdirectory(src)

target_link_libraries(Voxybox PUBLIC Voxymore::Core)
