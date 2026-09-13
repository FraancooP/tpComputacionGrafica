#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "Shader.h"
#include "Mesh.h"

#include <exception>
#include <iostream>
#include <stdexcept>

void errorGLFW(int codigo, const char *mensaje)
{
    std::cerr
        << "Error GLFW "
        << codigo
        << ": "
        << mensaje
        << '\n';
}

int main()
{
    
    glfwSetErrorCallback(errorGLFW);

    if (!glfwInit())
    {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *ventana = glfwCreateWindow(
        800,
        600,
        "Practico 02 - Incorporacion de Shader",
        nullptr,
        nullptr);

    if (ventana == nullptr)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(ventana);

    if (!gladLoadGL(glfwGetProcAddress) || !GLAD_GL_VERSION_4_5)
    {
        std::cerr << "No se pudo cargar OpenGL 4.5.\n";

        glfwDestroyWindow(ventana);
        glfwTerminate();

        return 1;
    }

    glfwSwapInterval(1);

    std::cout
        << "OpenGL: "
        << glGetString(GL_VERSION)
        << '\n';

    int resultado = 0;

    // Los objetos de este bloque se destruyen
    // antes de cerrar la ventana y el contexto.
    try
    {
        ResourceManager recursos("assets");
        Shader shader;

        const ShaderSource &fuentes =
            recursos.load_shader_source(
                "solid",
                "shaders/solid.vs",
                "shaders/solid.fs");

        if (!shader.compile_from_source(fuentes.vs, fuentes.fs))
        {
            throw std::runtime_error(
                "No se pudo preparar el programa de shaders.");
        }
        MeshData datos;

        datos.vertices = {
            // Posicion                 // Color
            {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
            {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f}};

        datos.indices = {
            0, 1, 2};

        Mesh malla;
        malla.load(datos);

        std::cout
            << "Vertices: " << datos.vertices.size()
            << " | Indices: " << malla.count()
            << '\n';

        glClearColor(0.10f, 0.12f, 0.16f, 1.0f);

        while (!glfwWindowShouldClose(ventana))
        {
            glfwPollEvents();

            if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(ventana, GLFW_TRUE);
            }

            int ancho = 0;
            int alto = 0;

            glfwGetFramebufferSize(ventana, &ancho, &alto);
            glViewport(0, 0, ancho, alto);

            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();

            glBindVertexArray(malla.vao());

            glDrawElements(
                GL_TRIANGLES,
                malla.count(),
                GL_UNSIGNED_INT,
                nullptr);

            glfwSwapBuffers(ventana);
        }

    } // Aqui se destruye shader y libera su programa.
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        resultado = 1;
    }

    glfwDestroyWindow(ventana);
    glfwTerminate();

    return resultado;
}