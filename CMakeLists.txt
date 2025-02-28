cmake_minimum_required(VERSION 3.14)
project(Rift_Runtime_Console VERSION 0.1.1)

add_library(
        Rift_Runtime_Console
        STATIC
        private/Engine/Runtime/UIConsole.cpp
)

target_include_directories(
        Rift_Runtime_Console
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private"
)

target_link_libraries(Rift_Runtime_Console RiftEngine_Core_Runtime Rift_UI_ImGui Rift_Runtime_Logging)