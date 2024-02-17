cmake_minimum_required(VERSION 3.3)
project(MagmaApp)


set(MAGMA_VULKAN ON)
set(MAGMA_OPENGL OFF)
set(MAGMA_EMSCRIPTEN OFF)


#add_compile_definitions(MAGMA_NO_LOGGING) #uncomment to stop all logging
#add_compile_definitions(MAGMA_NO_CORE_LOGGING) #uncomment to stop core logging
#add_compile_definitions(MAGMA_NO_CLIENT_LOGGING) #uncomment to stop client logging


add_subdirectory(vendor/Magma)
add_subdirectory(vendor/glm)

add_executable(MagmaApp 
app/src/main.cpp
 "app/src/Engine.h" "app/src/Engine.cpp" "app/src/Renderer.h" "app/src/Renderer.cpp" "app/src/Camera.h" "app/src/Camera.cpp" "app/src/Ray.h")


target_link_libraries(MagmaApp
PRIVATE
magma
glm
)