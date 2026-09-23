#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Aircraft.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "CameraSystem.h"
#include "InputHandler.h"

#include <cmath>
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
void redimensionarFramebuffer(
    GLFWwindow *ventana,
    int ancho,
    int alto)
{
    glViewport(0, 0, ancho, alto);

    auto *camara = static_cast<CameraSystem *>(
        glfwGetWindowUserPointer(ventana));

    if (camara != nullptr)
    {
        camara->set_viewport(ancho, alto);
    }
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
        "Practico 04 - Aeronave: cabeceo y rolido",
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

    // Aircraft y Shader se destruyen con el contexto activo.
    try
    {
        // --------------------------------------------------
        // Recursos y programa de shaders.
        // --------------------------------------------------

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

        // --------------------------------------------------
        // Modelo completo.
        // --------------------------------------------------

        Aircraft avion;
        avion.init(1.0f);

        std::cout
            << "Piezas del avion: "
            << avion.piezas().size()
            << '\n';

        const glm::vec3 referencia = avion.puntoReferencia();

        std::cout
            << "Referencia local de giro: ("
            << referencia.x << ", "
            << referencia.y << ", "
            << referencia.z << ")\n";

        std::cout
            << "Prueba: 6 segundos de cabeceo y "
            << "6 segundos de rolido, en repeticion.\n";

        // --------------------------------------------------
        // Uniforms.
        // --------------------------------------------------

        const int ubicacionModelo = shader.loc("uModel");
        const int ubicacionVista = shader.loc("uView");
        const int ubicacionProyeccion = shader.loc("uProjection");
        const int ubicacionColor = shader.loc("uColor");

        // Posicion del punto de referencia del avion en el mundo.
        const glm::vec3 posicionAvion(0.0f, 0.0f, 0.0f);

        // Usamos el tamaño real del framebuffer.
        int anchoInicial = 0;
        int altoInicial = 0;

        glfwGetFramebufferSize(
            ventana,
            &anchoInicial,
            &altoInicial);

        CameraSystem camara(anchoInicial, altoInicial);
        InputHandler entrada;

        // Asociamos la camara a la ventana para acceder desde el callback.
        glfwSetWindowUserPointer(ventana, &camara);

        glfwSetFramebufferSizeCallback(
            ventana,
            redimensionarFramebuffer);

        // Aplicamos tambien el viewport inicial.
        redimensionarFramebuffer(
            ventana,
            anchoInicial,
            altoInicial);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.10f, 0.12f, 0.16f, 1.0f);

        const double inicioAnimacion = glfwGetTime();
        const bool animarAvion = false;

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

            const CameraCommand comando = entrada.update(ventana);

            int ancho = 0;
            int alto = 0;

            glfwGetFramebufferSize(ventana, &ancho, &alto);

            if (ancho <= 0 || alto <= 0)
            {
                continue;
            }

            camara.update(
                posicionAvion,
                glm::vec3(0.0f),
                comando);

            const CameraData &datosCamara = camara.data();

            shader.set_uniform(
                ubicacionVista,
                datosCamara.view);

            shader.set_uniform(
                ubicacionProyeccion,
                datosCamara.projection);

            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

            // --------------------------------------------------
            // Prueba alternada de cabeceo y rolido.
            // --------------------------------------------------

            const double tiempo =
                glfwGetTime() - inicioAnimacion;

            const float duracion = 6.0f;

            const float fase = static_cast<float>(
                std::fmod(tiempo, 12.0));

            float cabeceo = 0.0f;
            float rolido = 0.0f;

            if (fase < duracion)
            {
                const float avance = fase / duracion;

                cabeceo =
                    glm::radians(15.0f) *
                    std::sin(glm::radians(360.0f) * avance);
            }
            else
            {
                const float avance =
                    (fase - duracion) / duracion;

                rolido =
                    glm::radians(25.0f) *
                    std::sin(glm::radians(360.0f) * avance);
            }
            if (!animarAvion)
            {
                cabeceo = 0.0f;
                rolido = 0.0f;
            }

            // --------------------------------------------------
            // Pose comun a todas las piezas.
            // T(posicion) * Rx * Rz * T(-referencia).
            // --------------------------------------------------

            glm::mat4 pose = glm::mat4(1.0f);

            pose = glm::translate(
                pose,
                posicionAvion);

            pose = glm::rotate(
                pose,
                cabeceo,
                glm::vec3(1.0f, 0.0f, 0.0f));

            pose = glm::rotate(
                pose,
                rolido,
                glm::vec3(0.0f, 0.0f, 1.0f));

            pose = glm::translate(
                pose,
                -referencia);

            // --------------------------------------------------
            // Dibujamos las siete piezas.
            // --------------------------------------------------

            shader.use();

            for (const PiezaAvion &pieza : avion.piezas())
            {
                const Mesh &malla = avion.malla(pieza.tipo);

                shader.set_uniform(
                    ubicacionModelo,
                    pose * pieza.local);

                shader.set_uniform(
                    ubicacionColor,
                    pieza.color);

                glBindVertexArray(malla.vao());

                glDrawElements(
                    GL_TRIANGLES,
                    malla.count(),
                    GL_UNSIGNED_INT,
                    nullptr);
            }

            glfwSwapBuffers(ventana);
        }
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        resultado = 1;
    }

    glfwSetFramebufferSizeCallback(ventana, nullptr);
    glfwSetWindowUserPointer(ventana, nullptr);

    glfwDestroyWindow(ventana);
    glfwTerminate();

    return resultado;
}