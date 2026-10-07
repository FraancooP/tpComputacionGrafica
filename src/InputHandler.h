#pragma once

#include "CameraCommand.h"
#include <dlfdm/defines.h>

// Declaracion adelantada: el encabezado solo necesita el puntero.
struct GLFWwindow;

class InputHandler
{
public:
    // Consultar una vez por cuadro, despues de glfwPollEvents().
    CameraCommand update(GLFWwindow *ventana);
    void set_controls(const dlfdm::ControlInputs &controles)
    {
        controles_ = controles;
    }

    const dlfdm::ControlInputs &controls() const
    {
        return controles_;
    }

private:
    dlfdm::ControlInputs controles_{};

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