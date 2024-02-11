cmake_minimum_required(VERSION 3.3)
project(MagmaApp)


set(MAGMA_VULKAN ON)
set(MAGMA_OPENGL OFF)
set(MAGMA_EMSCRIPTEN OFF)


#add_compile_definitions(MAGMA_NO_LOGGING) #uncomment to stop all logging
#add_compile_definitions(MAGMA_NO_CORE_LOGGING) #uncomment to stop core logging
#add_compile_definitions(MAGMA_NO_CLIENT_LOGGING) #uncomment to stop client logging


add_subdirectory(vendor/Magma)

add_executable(MagmaApp 
app/src/main.cpp
 "app/src/Engine.h" "app/src/Engine.cpp")


target_link_libraries(MagmaApp
PRIVATE
magma
)