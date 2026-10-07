#pragma once

#include "CameraCommand.h"
#include <dlfdm/defines.h>

// Declaracion adelantada: el encabezado solo necesita el puntero.
struct GLFWwindow;

class InputHandler
{
public:
    // Consultar una vez por cuadro, despues de glfwPollEvents().
    // Actualiza teclado y mouse una vez por cuadro.
    void update(GLFWwindow *ventana, float dt);

    // Devuelve el comando de camara calculado en update().
    CameraCommand camera_cmd() const
    {
        return comandoCamara_;
    }

    // Obtiene los limites de deflexion del modelo del profesor.
    void set_limits(const dlfdm::AircraftParameters &parametros);
    void set_controls(const dlfdm::ControlInputs &controles)
    {
        controles_ = controles;
    }

    const dlfdm::ControlInputs &controls() const
    {
        return controles_;
    }

private:
    // Reutiliza el procesamiento de mouse que ya teniamos.
    CameraCommand leerCamara(GLFWwindow *ventana);

    CameraCommand comandoCamara_{};

    // Limites expresados en radianes.
    float minimoElevador_ = 0.0f;
    float maximoElevador_ = 0.0f;

    float minimoAleron_ = 0.0f;
    float maximoAleron_ = 0.0f;

    float maximoTimon_ = 0.0f;
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