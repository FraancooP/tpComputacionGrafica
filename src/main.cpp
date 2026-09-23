#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Aircraft.h"
#include "ResourceManager.h"
#include "Shader.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

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
        "Practico 04 - Aeronave",
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

    // Los recursos se destruyen al salir de este bloque,
    // antes de cerrar el contexto de OpenGL.
    try
    {
        // --------------------------------------------------
        // Fuentes y programa de shaders.
        // --------------------------------------------------

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

        // --------------------------------------------------
        // Modelo del avion.
        // --------------------------------------------------

        const float L = 1.0f;

        Aircraft avion;
        avion.init(L);

        // Copiamos las descripciones de las piezas.
        // Las mallas siguen perteneciendo a Aircraft.
        std::vector<PiezaAvion> piezas(
            avion.piezas().begin(),
            avion.piezas().end()
        );

        // Conservamos el cono adicional de cola de tu main.
        // Sus medidas y transformacion son las que tenias.
        const float largoNariz = L / 5.0f;
        const float largoFuselaje = L;

        const float largoCola = L / 5.0f;
        const float radioCola = L / 10.0f;

        const float posicionZCola =
            largoNariz + largoFuselaje * 0.88f;

        glm::mat4 localCola = glm::mat4(1.0f);

        localCola = glm::translate(
            localCola,
            glm::vec3(
                0.0f,
                0.0f,
                posicionZCola + largoCola * 0.5f
            )
        );

        localCola = glm::rotate(
            localCola,
            glm::radians(-90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        localCola = glm::scale(
            localCola,
            glm::vec3(
                radioCola,
                largoCola,
                radioCola
            )
        );

        piezas.push_back(
            PiezaAvion{
                TipoMalla::Cono,
                localCola,
                glm::vec3(0.5f, 0.5f, 0.5f)
            }
        );

        // --------------------------------------------------
        // Uniforms.
        // --------------------------------------------------

        const int ubicacionModelo = shader.loc("uModel");
        const int ubicacionAjuste = shader.loc("uAjuste");
        const int ubicacionColor = shader.loc("uColor");

        // --------------------------------------------------
        // Presentacion y referencia de movimiento.
        // --------------------------------------------------

        const glm::vec3 posicionAvion(0.0f, 0.0f, 0.0f);

        // Conservamos la orientacion que elegiste.
        glm::mat4 presentacion = glm::mat4(1.0f);

        presentacion = glm::rotate(
            presentacion,
            glm::radians(55.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        presentacion = glm::rotate(
            presentacion,
            glm::radians(45.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.10f, 0.12f, 0.16f, 1.0f);

        const double inicioAnimacion = glfwGetTime();

        // --------------------------------------------------
        // Ciclo de dibujo.
        // --------------------------------------------------

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

            if (ancho <= 0 || alto <= 0)
            {
                continue;
            }

            glViewport(0, 0, ancho, alto);

            const float relacion =
                static_cast<float>(alto) /
                static_cast<float>(ancho);

            const glm::mat4 ajuste = glm::scale(
                glm::mat4(1.0f),
                glm::vec3(relacion, 1.0f, -1.0f)
            );

            shader.set_uniform(ubicacionAjuste, ajuste);

            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT
            );

            // Cabeceo entre -15 y +15 grados.
            const float tiempo = static_cast<float>(
                glfwGetTime() - inicioAnimacion
            );

            const float cabeceo =
                glm::radians(15.0f) * std::sin(tiempo);

            // Pose = T(posicion) * R(cabeceo) * T(-referencia).
            glm::mat4 pose = glm::mat4(1.0f);

            pose = glm::translate(
                pose,
                posicionAvion
            );

            pose = glm::rotate(
                pose,
                cabeceo,
                glm::vec3(1.0f, 0.0f, 0.0f)
            );

            pose = glm::translate(
                pose,
                -avion.puntoReferencia()
            );

            shader.use();

            // Todas las piezas reciben la misma pose.
            for (const PiezaAvion& pieza : piezas)
            {
                const Mesh& malla = avion.malla(pieza.tipo);

                shader.set_uniform(
                    ubicacionModelo,
                    presentacion * pose * pieza.local
                );

                shader.set_uniform(
                    ubicacionColor,
                    pieza.color
                );

                glBindVertexArray(malla.vao());

                glDrawElements(
                    GL_TRIANGLES,
                    malla.count(),
                    GL_UNSIGNED_INT,
                    nullptr
                );
            }

            glfwSwapBuffers(ventana);
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        resultado = 1;
    }

    glfwDestroyWindow(ventana);
    glfwTerminate();

    return resultado;
}