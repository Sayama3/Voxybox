cmake_minimum_required(VERSION 3.24)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(VoxymoreCore)
target_link_libraries(Voxybox PUBLIC VoxymoreCore)
target_include_directories(Voxybox PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/VoxymoreCore"
        )