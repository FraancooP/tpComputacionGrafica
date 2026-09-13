#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "Shader.h"

#include <exception>
#include <iostream>
#include <stdexcept>

void errorGLFW(int codigo, const char* mensaje)
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

    GLFWwindow* ventana = glfwCreateWindow(
        800,
        600,
        "Practico 02 - Incorporacion de Shader",
        nullptr,
        nullptr
    );

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

        const ShaderSource& fuentes =
            recursos.load_shader_source(
                "solid",
                "shaders/solid.vs",
                "shaders/solid.fs"
            );

        if (!shader.compile_from_source(fuentes.vs, fuentes.fs))
        {
            throw std::runtime_error(
                "No se pudo preparar el programa de shaders."
            );
        }

        const float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        GLuint vao = 0;
        GLuint vbo = 0;

        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbo);

        glNamedBufferData(
            vbo,
            static_cast<GLsizeiptr>(sizeof(vertices)),
            vertices,
            GL_STATIC_DRAW
        );

        glVertexArrayVertexBuffer(
            vao,
            0,
            vbo,
            0,
            static_cast<GLsizei>(3 * sizeof(float))
        );

        glVertexArrayAttribFormat(
            vao,
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0
        );

        glVertexArrayAttribBinding(vao, 0, 0);
        glEnableVertexArrayAttrib(vao, 0);

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

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(ventana);
        }

        // Estos recursos pasaran a la clase Mesh
        // en el siguiente paso.
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);

    } // Aqui se destruye shader y libera su programa.
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        resultado = 1;
    }

    glfwDestroyWindow(ventana);
    glfwTerminate();

    return resultado;
}