cmake_minimum_required(VERSION 3.20)

project(TrabajoIntegradorCG LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# El ejecutable quedara dentro de bin, en la carpeta de compilacion.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# GLFW: ventana, contexto OpenGL y eventos.
include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Para Ubuntu usamos X11.
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

# GLM: vectores y matrices para las transformaciones.
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)
# Modelo de dinamica de vuelo proporcionado por el profe
add_library(dlfdm STATIC
    libs/dlfdm/src/dlfdm/aerodynamicsmodel.cpp
    libs/dlfdm/src/dlfdm/aircraftdynamics.cpp
    libs/dlfdm/src/dlfdm/fdmsolver.cpp
    libs/dlfdm/src/dlfdm/propulsionmodel.cpp
    libs/dlfdm/src/dlfdm/models/aircraft/jettrainer.cpp
    libs/dlfdm/src/dlfdm/models/propulsion/turbofan.cpp
    libs/dlfdm/src/dlfdm/models/propulsion/turbojet.cpp
)

target_include_directories(dlfdm PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/dlfdm/include"
)

target_link_libraries(dlfdm PUBLIC glm::glm)

target_compile_features(dlfdm PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(dlfdm PRIVATE /utf-8)
endif()



# GLAD: cargador de funciones OpenGL.
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# Nuestro programa.
add_executable(cgyav
    src/main.cpp
    src/ResourceManager.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Primitives.cpp
    src/Aircraft.cpp
    src/CameraSystem.cpp
    src/InputHandler.cpp
)

target_link_libraries(cgyav PRIVATE
    glad
    glfw
    glm::glm
    dlfdm
)

# Advertencias del compilador.
if(MSVC)
    target_compile_options(cgyav PRIVATE /W4 /utf-8)
else()
    target_compile_options(cgyav PRIVATE -Wall -Wextra -Wpedantic)
endif()





# Prueba independiente incluida por el modelo propuesto
add_executable(dlfdm-test EXCLUDE_FROM_ALL
    libs/dlfdm/test/main.cpp
)

target_link_libraries(dlfdm-test PRIVATE dlfdm)

if(MSVC)
    target_compile_options(dlfdm-test PRIVATE /utf-8)
endif()