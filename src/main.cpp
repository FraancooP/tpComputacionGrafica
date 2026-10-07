#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <dlfdm/fdmsolver.h>
#include <dlfdm/models/aircraft/jettrainer.h>

#include "Aircraft.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "CameraSystem.h"
#include "InputHandler.h"
#include "FlightData.h"

#include <algorithm>
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

void redimensionarFramebuffer(
    GLFWwindow* ventana,
    int ancho,
    int alto)
{
    glViewport(0, 0, ancho, alto);

    auto* camara = static_cast<CameraSystem*>(
        glfwGetWindowUserPointer(ventana)
    );

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

    GLFWwindow* ventana = glfwCreateWindow(
        800,
        600,
        "Practico 06 - Dinamica de vuelo",
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

    // Los recursos graficos se destruyen con el contexto activo.
    try
    {
        // 1. Recursos y shaders.

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

        // 2. Modelo geometrico del avion.

        Aircraft avion;
        avion.init(1.0f);

        const glm::vec3 referencia = avion.puntoReferencia();

        std::cout
            << "Piezas del avion: "
            << avion.piezas().size()
            << '\n';

        std::cout
            << "Referencia local de giro: ("
            << referencia.x << ", "
            << referencia.y << ", "
            << referencia.z << ")\n";

        // 3. Ubicaciones de los uniforms.

        const int ubicacionModelo = shader.loc("uModel");
        const int ubicacionVista = shader.loc("uView");
        const int ubicacionProyeccion = shader.loc("uProjection");
        const int ubicacionColor = shader.loc("uColor");

        // 4. Camara y entrada.

        int anchoInicial = 0;
        int altoInicial = 0;

        glfwGetFramebufferSize(
            ventana,
            &anchoInicial,
            &altoInicial
        );

        CameraSystem camara(anchoInicial, altoInicial);
        InputHandler entrada;

        glfwSetWindowUserPointer(ventana, &camara);

        glfwSetFramebufferSizeCallback(
            ventana,
            redimensionarFramebuffer
        );

        redimensionarFramebuffer(
            ventana,
            anchoInicial,
            altoInicial
        );

        // 5. Modelo fisico y condicion inicial.

        constexpr float pasoFdm = 1.0f / 120.0f;
        const double dt = static_cast<double>(pasoFdm);

        const dlfdm::AircraftParameters parametros =
            dlfdm::jettrainer::load_model();

        dlfdm::FDMSolver fdm(parametros, pasoFdm);

        const dlfdm::TrimPoint trim =
            dlfdm::jettrainer::get_trim_condition(
                dlfdm::jettrainer::TrimCondition::kISA5000TAS150
            );

        // El equilibrio requiere cargar estado Y mandos.
        fdm.setState(trim.state);
        entrada.set_controls(trim.controls);

        std::cout
            << "FDM iniciado en la condicion de equilibrio del profesor.\n";

        // 6. Configuracion de dibujo.

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.10f, 0.12f, 0.16f, 1.0f);

        // 7. Reloj y acumulador.

        double antes = glfwGetTime();
        double acumulador = 0.0;
        double proximoInforme = antes + 1.0;

        // 8. Bucle principal.

        while (!glfwWindowShouldClose(ventana))
        {
            glfwPollEvents();

            const double ahora = glfwGetTime();
            const double frame_dt = ahora - antes;
            antes = ahora;

            const double tiempoParaSimular =
                std::min(frame_dt, 0.25);

            // Entrada: una lectura por cuadro.

            if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(ventana, GLFW_TRUE);
            }

            if (glfwWindowShouldClose(ventana))
            {
                break;
            }

            const CameraCommand comando = entrada.update(ventana);

            // Fisica: cero, uno o varios pasos fijos por cuadro.

            acumulador += tiempoParaSimular;

            while (acumulador >= dt)
            {
                fdm.update(entrada.controls());
                acumulador -= dt;
            }

            // Recuperamos el estado y lo convertimos a la escena.

            const dlfdm::AircraftState& estadoNed = fdm.getState();
            const FlightData vuelo = to_world(estadoNed);

            const glm::vec3 posicionAvion = vuelo.position;

            // Informe para comprobar el movimiento.

            if (ahora >= proximoInforme)
            {
                std::cout
                    << "Altura: " << vuelo.position.y << " m"
                    << " | Velocidad: "
                    << glm::length(estadoNed.body_velocity)
                    << " m/s"
                    << " | Posicion: "
                    << vuelo.position.x << ", "
                    << vuelo.position.y << ", "
                    << vuelo.position.z
                    << '\n';

                proximoInforme = ahora + 1.0;
            }

            // La camara sigue la posicion calculada por el FDM.

            camara.update(
                posicionAvion,
                glm::vec3(vuelo.phi, vuelo.theta, vuelo.psi),
                comando
            );

            // Si no hay superficie visible, omitimos el dibujo.
            // La fisica ya se actualizo antes de llegar a este punto.

            int ancho = 0;
            int alto = 0;

            glfwGetFramebufferSize(ventana, &ancho, &alto);

            if (ancho <= 0 || alto <= 0)
            {
                continue;
            }

            const CameraData& datosCamara = camara.data();

            shader.use();

            shader.set_uniform(
                ubicacionVista,
                datosCamara.view
            );

            shader.set_uniform(
                ubicacionProyeccion,
                datosCamara.projection
            );

            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT
            );

            // Pose del avion:
            // T(posicion) * Ry(psi) * Rx(theta) * Rz(phi)
            // * T(-referencia).

            glm::mat4 pose = glm::translate(
                glm::mat4(1.0f),
                posicionAvion
            );

            // Guinada.
            pose = glm::rotate(
                pose,
                vuelo.psi,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            // Cabeceo.
            pose = glm::rotate(
                pose,
                vuelo.theta,
                glm::vec3(1.0f, 0.0f, 0.0f)
            );

            // Alabeo.
            pose = glm::rotate(
                pose,
                vuelo.phi,
                glm::vec3(0.0f, 0.0f, 1.0f)
            );

            // El punto de referencia del modelo coincide
            // con la posicion entregada por el FDM.
            pose = glm::translate(
                pose,
                -referencia
            );

            // Dibujamos las piezas con la pose comun.

            for (const PiezaAvion& pieza : avion.piezas())
            {
                const Mesh& malla = avion.malla(pieza.tipo);

                shader.set_uniform(
                    ubicacionModelo,
                    pose * pieza.local
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

    glfwSetFramebufferSizeCallback(ventana, nullptr);
    glfwSetWindowUserPointer(ventana, nullptr);

    glfwDestroyWindow(ventana);
    glfwTerminate();

    return resultado;
}