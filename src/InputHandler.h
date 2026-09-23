#pragma once

#include "CameraCommand.h"

// Declaracion adelantada: el encabezado solo necesita el puntero.
struct GLFWwindow;

class InputHandler
{
public:
    // Consultar una vez por cuadro, despues de glfwPollEvents().
    CameraCommand update(GLFWwindow* ventana);

private:
    enum class Modo
    {
        Ninguno,
        Orbita,
        Distancia
    };

    bool tengoAnterior_ = false;

    double xAnterior_ = 0.0;
    double yAnterior_ = 0.0;

    Modo modoAnterior_ = Modo::Ninguno;

    // Radianes por unidad de desplazamiento del cursor.
    float sensibilidadAngular_ = 0.005f;

    // Unidades de escena por unidad de desplazamiento del cursor.
    float sensibilidadDistancia_ = 0.01f;
};