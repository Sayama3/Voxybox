cmake_minimum_required(VERSION 3.24)

# set enable the debugger (when the debug mode is enable?)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

IF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    add_compile_definitions(VXM_Debug)
    add_compile_definitions(Debug)
ENDIF()

set(VoxymoreCore_SHARED_BUILD ON)
add_subdirectory(lib/VoxymoreCore)

project(VoxyboxProject)

add_executable(Voxybox)

set(CMAKE_CXX_STANDARD 20)
target_compile_features(Voxybox PUBLIC cxx_std_20)

include_directories(include)
add_subdirectory(src)

target_link_libraries(Voxybox PUBLIC Voxymore::Core)
target_precompile_headers(Voxybox REUSE_FROM Voxymore::Core)
