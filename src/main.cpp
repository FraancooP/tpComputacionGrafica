#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Primitives.h"

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
        // ESTO ES PRIMITIVAS SOLAS!!=====================================================================
        /*
         MeshData datos;

        datos.vertices = {
             // Posicion                 // Color
             {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
             {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
             {0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f}};

         datos.indices = {
             0, 1, 2};

         Crea un cubo.
         MeshData datos = primitives::cube();

        MeshData datos = primitives::cylinder(
            0.5f,
            1.0f,
            32U);

        MeshData datos = primitives::cone(
            0.5f,
            60.0f,
            32U);

        Mesh malla;
        malla.load(datos);

        std::cout
            << "Vertices: " << datos.vertices.size()
            << " | Indices: " << malla.count()
            << '\n';
        */
        // ESTO ES PRIMITIVAS SOLAS!!=====================================================================
        // Esto es las 3 con rotacion=====================================================================
        /*
                // Generamos los datos una sola vez.
                const MeshData datosCubo = primitives::cube();

                const MeshData datosCilindro =
                    primitives::cylinder(0.5f, 1.0f, 32U);

                const MeshData datosCono =
                    primitives::cone(0.5f, 60.0f, 32U);

                // Cada Mesh administra los recursos de una primitiva.
                Mesh cubo;
                Mesh cilindro;
                Mesh cono;

                cubo.load(datosCubo);
                cilindro.load(datosCilindro);
                cono.load(datosCono);

                std::cout
                    << "Cubo: "
                    << datosCubo.vertices.size() << " vertices, "
                    << cubo.count() << " indices\n";

                std::cout
                    << "Cilindro: "
                    << datosCilindro.vertices.size() << " vertices, "
                    << cilindro.count() << " indices\n";

                std::cout
                    << "Cono: "
                    << datosCono.vertices.size() << " vertices, "
                    << cono.count() << " indices\n";

                // Consultamos las ubicaciones una sola vez.
                const int ubicacionModelo = shader.loc("uModel");
                const int ubicacionAjuste = shader.loc("uAjuste");
                const int ubicacionColor = shader.loc("uColor");

                // Punto de partida para medir el tiempo de la animacion.
                const double inicioAnimacion = glfwGetTime();
        //Esto es las 3 con rotacion=====================================================================

        */
        // ESTO ES AVION===================================================================================

        // Medidas provisionales para empezar el modelo.
        const float L = 1.0f;

        const float largoNariz = L / 5.0f;
        const float radioNariz = L / 10.0f;
        const float largoFuselaje = L;
        const float radioFuselaje = radioNariz;
        const float largoAla = L / 2.0f;
        const float cuerdaAla = L / 4.0f;
        const float espesorAla = L / 40.0f;
        const float anchoEstabilizador = L / 2.0f;
        const float cuerdaEstabilizador = L / 5.0f;
        const float espesorEstabilizador = L / 40.0f;

        const float alturaDeriva = L / 3.0f;
        const float cuerdaDeriva = L / 5.0f;
        const float espesorDeriva = L / 40.0f;

        const float posicionZAla = largoNariz + largoFuselaje * 0.45f;
        const float posicionZCola = largoNariz + largoFuselaje * 0.88f;
        // Con radio 1 y apertura de 90 grados,
        // el cono generado tiene altura 1.
        const MeshData datosNariz = primitives::cone(
            1.0f,
            90.0f,
            32U);

        Mesh nariz;
        nariz.load(datosNariz);

        const MeshData datosFuselaje = primitives::cylinder(
            1.0f,
            1.0f,
            32U);

        Mesh fuselaje;
        fuselaje.load(datosFuselaje);

        const MeshData datosAla = primitives::cube();
        Mesh mallaAla;
        mallaAla.load(datosAla);

        const int ubicacionModelo = shader.loc("uModel");
        const int ubicacionAjuste = shader.loc("uAjuste");
        const int ubicacionColor = shader.loc("uColor");

        // Transformaciones locales de cada parte del avion.
        glm::mat4 localNariz = glm::mat4(1.0f);
        glm::mat4 localFuselaje = glm::mat4(1.0f);
        glm::mat4 localAlaIzquierda = glm::mat4(1.0f);
        glm::mat4 localAlaDerecha = glm::mat4(1.0f);
        glm::mat4 localEstabilizador = glm::mat4(1.0f);
        glm::mat4 localDeriva = glm::mat4(1.0f);

        // NARIZ

        localNariz = glm::translate(
            localNariz,
            glm::vec3(0.0f, 0.0f, largoNariz * 0.5f));

        localNariz = glm::rotate(
            localNariz,
            glm::radians(-90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));

        localNariz = glm::scale(
            localNariz,
            glm::vec3(radioNariz, largoNariz, radioNariz));

        // FUSELAJE
        // Ubicamos el cilindro al centro
        localFuselaje = glm::translate(
            localFuselaje,
            glm::vec3(0.0f, 0.0f, largoNariz + largoFuselaje * 0.5f));

        localFuselaje = glm::rotate(
            localFuselaje,
            glm::radians(-90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));

        localFuselaje = glm::scale(
            localFuselaje,
            glm::vec3(radioFuselaje, largoFuselaje, radioFuselaje));

        // ALA IZQUIERDA
        localAlaIzquierda = glm::translate(
            localAlaIzquierda,
            glm::vec3(-largoAla * 0.5f, 0.0f, posicionZAla));

        localAlaIzquierda = glm::scale(
            localAlaIzquierda,
            glm::vec3(largoAla, espesorAla, cuerdaAla));

        // ALA DERECHA
        localAlaDerecha = glm::translate(
            localAlaDerecha,
            glm::vec3(largoAla * 0.5f, 0.0f, posicionZAla));
        localAlaDerecha = glm::scale(
            localAlaDerecha,
            glm::vec3(largoAla, espesorAla, cuerdaAla));

        // ESTABILIZADOR
        localEstabilizador = glm::translate(
            localEstabilizador,
            glm::vec3(
                0.0f,
                0.0f,
                posicionZCola));

        localEstabilizador = glm::scale(
            localEstabilizador,
            glm::vec3(
                anchoEstabilizador,
                espesorEstabilizador,
                cuerdaEstabilizador));

        // DERIVA

        localDeriva = glm::translate(
            localDeriva,
            glm::vec3(
                0.0f,
                alturaDeriva * 0.5f,
                posicionZCola));

        localDeriva = glm::scale(
            localDeriva,
            glm::vec3(
                espesorDeriva,
                alturaDeriva,
                cuerdaDeriva));

        // Orientacion del conjunto para observarlo de costado.
        // No forma parte de las medidas ni del armado de la nariz.
        glm::mat4 presentacion = glm::mat4(1.0f);

        presentacion = glm::rotate(
            presentacion,
            glm::radians(55.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        presentacion = glm::rotate(
            presentacion,
            glm::radians(20.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.10f, 0.12f, 0.16f, 1.0f);

        // Empezamos con la matriz identidad.
        glm::mat4 modelo = glm::mat4(1.0f);

        // M  = T * Rx * Ry * S

        // Posicion del objeto.
        modelo = glm::translate(
            modelo,
            glm::vec3(0.0f, 0.0f, 0.0f));

        // Orientacion alrededor de X.
        modelo = glm::rotate(
            modelo,
            glm::radians(25.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));

        // Orientacion alrededor de Y.
        modelo = glm::rotate(
            modelo,
            glm::radians(-35.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        // Escalado del objeto.
        modelo = glm::scale(
            modelo,
            glm::vec3(0.6f, 0.6f, 0.6f));

        shader.set_uniform(ubicacionModelo, modelo);
        shader.set_uniform(
            ubicacionColor,
            glm::vec3(1.0f, 0.5f, 0.2f));

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

            // Al minimizar, el framebuffer puede tener dimensiones cero.
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
                glm::vec3(relacion, 1.0f, -1.0f));

            shader.set_uniform(ubicacionAjuste, ajuste);

            // Esto es las 3 con rotacion=====================================================================
            /*
            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

            const float tiempo = static_cast<float>(
                glfwGetTime() - inicioAnimacion);

            // Velocidad: 30 grados por segundo.
            const float angulo = glm::radians(30.0f) * tiempo;

            shader.use();

            // --------------------------------------------------
            // CUBO: a la izquierda.
            // --------------------------------------------------

            glm::mat4 modeloCubo = glm::mat4(1.0f);

            modeloCubo = glm::translate(
                modeloCubo,
                glm::vec3(-0.9f, 0.15f, 0.0f));

            modeloCubo = glm::rotate(
                modeloCubo,
                glm::radians(25.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            modeloCubo = glm::rotate(
                modeloCubo,
                angulo,
                glm::vec3(0.0f, 1.0f, 0.0f));

            modeloCubo = glm::scale(
                modeloCubo,
                glm::vec3(0.42f, 0.42f, 0.42f));

            shader.set_uniform(ubicacionModelo, modeloCubo);

            shader.set_uniform(
                ubicacionColor,
                glm::vec3(1.0f, 0.5f, 0.2f));

            glBindVertexArray(cubo.vao());

            glDrawElements(
                GL_TRIANGLES,
                cubo.count(),
                GL_UNSIGNED_INT,
                nullptr);

            // --------------------------------------------------
            // CILINDRO: en el centro.
            // --------------------------------------------------

            glm::mat4 modeloCilindro = glm::mat4(1.0f);

            modeloCilindro = glm::translate(
                modeloCilindro,
                glm::vec3(0.0f, -0.15f, 0.0f));

            modeloCilindro = glm::rotate(
                modeloCilindro,
                angulo,
                glm::vec3(1.0f, 0.0f, 0.0f));

            modeloCilindro = glm::scale(
                modeloCilindro,
                glm::vec3(0.42f, 0.65f, 0.42f));

            shader.set_uniform(ubicacionModelo, modeloCilindro);

            shader.set_uniform(
                ubicacionColor,
                glm::vec3(0.2f, 0.8f, 0.4f));

            glBindVertexArray(cilindro.vao());

            glDrawElements(
                GL_TRIANGLES,
                cilindro.count(),
                GL_UNSIGNED_INT,
                nullptr);

            // --------------------------------------------------
            // CONO: a la derecha.
            // --------------------------------------------------

            glm::mat4 modeloCono = glm::mat4(1.0f);

            modeloCono = glm::translate(
                modeloCono,
                glm::vec3(0.9f, 0.15f, 0.0f));

            modeloCono = glm::rotate(
                modeloCono,
                glm::radians(25.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            modeloCono = glm::rotate(
                modeloCono,
                -angulo,
                glm::vec3(0.0f, 0.0f, 1.0f));

            modeloCono = glm::scale(
                modeloCono,
                glm::vec3(0.6f, 0.7f, 0.6f));

            shader.set_uniform(ubicacionModelo, modeloCono);

            shader.set_uniform(
                ubicacionColor,
                glm::vec3(0.3f, 0.6f, 1.0f));

            glBindVertexArray(cono.vao());

            glDrawElements(
                GL_TRIANGLES,
                cono.count(),
                GL_UNSIGNED_INT,
                nullptr);

            // Presentamos la imagen cuando terminamos las tres piezas.
            glfwSwapBuffers(ventana);
            */
            // ESTO ES AVION===================================================================================
            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

            shader.use();

            shader.set_uniform(
                ubicacionModelo,
                presentacion * localNariz);

            shader.set_uniform(
                ubicacionColor,
                glm::vec3(1.0f, 0.5f, 0.2f));

            glBindVertexArray(nariz.vao());

            glDrawElements(
                GL_TRIANGLES,
                nariz.count(),
                GL_UNSIGNED_INT,
                nullptr);

            // Fuselaje.
            shader.set_uniform(
                ubicacionModelo,
                presentacion * localFuselaje);

            shader.set_uniform(
                ubicacionColor,
                glm::vec3(0.7f, 0.75f, 0.8f));

            glBindVertexArray(fuselaje.vao());

            glDrawElements(
                GL_TRIANGLES,
                fuselaje.count(),
                GL_UNSIGNED_INT,
                nullptr);

            // Las dos alas usan la misma malla y el mismo color.
            shader.set_uniform(
                ubicacionColor,
                glm::vec3(0.25f, 0.55f, 0.9f));

            glBindVertexArray(mallaAla.vao());

            // Ala izquierda.
            shader.set_uniform(
                ubicacionModelo,
                presentacion * localAlaIzquierda);

            glDrawElements(
                GL_TRIANGLES,
                mallaAla.count(),
                GL_UNSIGNED_INT,
                nullptr);

            // Ala derecha.
            shader.set_uniform(
                ubicacionModelo,
                presentacion * localAlaDerecha);

            glDrawElements(
                GL_TRIANGLES,
                mallaAla.count(),
                GL_UNSIGNED_INT,
                nullptr);


            
            
            // Reutilizamos la malla de cubo de las alas.
            glBindVertexArray(mallaAla.vao());

            // Estabilizador horizontal.
            shader.set_uniform(
                ubicacionModelo,
                presentacion * localEstabilizador);

            shader.set_uniform(
                ubicacionColor,
                glm::vec3(0.3f, 0.8f, 0.45f));

            glDrawElements(
                GL_TRIANGLES,
                mallaAla.count(),
                GL_UNSIGNED_INT,
                nullptr);

            // Deriva vertical.
            shader.set_uniform(
                ubicacionModelo,
                presentacion * localDeriva);

            shader.set_uniform(
                ubicacionColor,
                glm::vec3(0.95f, 0.8f, 0.2f));

            glDrawElements(
                GL_TRIANGLES,
                mallaAla.count(),
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
