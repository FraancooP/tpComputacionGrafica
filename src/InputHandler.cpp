#include "InputHandler.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>

CameraCommand InputHandler::leerCamara(GLFWwindow* ventana)
{
    CameraCommand comando{};

    if (ventana == nullptr)
    {
        tengoAnterior_ = false;
        modoAnterior_ = Modo::Ninguno;
        return comando;
    }

    double xActual = 0.0;
    double yActual = 0.0;
    glfwGetCursorPos(ventana, &xActual, &yActual);

    Modo modoActual = Modo::Ninguno;

    if (glfwGetMouseButton(ventana, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        modoActual = Modo::Orbita;
    }
    else if (glfwGetMouseButton(ventana, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        modoActual = Modo::Distancia;
    }

    // Al comenzar o cambiar de gesto solo guardamos la posicion. De este modo
    // el primer cuadro no produce un salto por una coordenada anterior obsoleta.
    if (tengoAnterior_ && modoActual == modoAnterior_)
    {
        const double deltaX = xActual - xAnterior_;
        const double deltaY = yActual - yAnterior_;

        if (modoActual == Modo::Orbita)
        {
            comando.yaw_delta = static_cast<float>(-deltaX) * sensibilidadAngular_;
            comando.pitch_delta = static_cast<float>(deltaY) * sensibilidadAngular_;
        }
        else if (modoActual == Modo::Distancia)
        {
            comando.dist_delta = static_cast<float>(deltaY) * sensibilidadDistancia_;
        }
    }

    xAnterior_ = xActual;
    yAnterior_ = yActual;
    modoAnterior_ = modoActual;
    tengoAnterior_ = true;

    return comando;
}
void InputHandler::set_limits(
    const dlfdm::AircraftParameters& parametros)
{
    minimoElevador_ = parametros.min_elevator;
    maximoElevador_ = parametros.max_elevator;

    minimoAleron_ = parametros.min_aileron;
    maximoAleron_ = parametros.max_aileron;

    maximoTimon_ = parametros.max_rudder;
}

void InputHandler::update(GLFWwindow* ventana, float dt)
{
    // El mouse ya entrega desplazamientos entre lecturas.
    comandoCamara_ = leerCamara(ventana);

    // Si estamos trabajando en otra ventana, no procesamos mandos.
    if (glfwGetWindowAttrib(ventana, GLFW_FOCUSED) != GLFW_TRUE)
    {
        return;
    }

    // Devuelve:
    // +1 si se mantiene la tecla positiva.
    // -1 si se mantiene la tecla negativa.
    //  0 si no se mantiene ninguna, o si se mantienen ambas.
    const auto eje = [ventana](int positiva, int negativa) -> float
    {
        const bool mas =
            glfwGetKey(ventana, positiva) == GLFW_PRESS;

        const bool menos =
            glfwGetKey(ventana, negativa) == GLFW_PRESS;

        return static_cast<float>(mas) -
               static_cast<float>(menos);
    };

    // Sensibilidad del teclado elegida para nuestra aplicacion.
    // Las superficies cambian 10 grados por segundo.
    const float velocidadSuperficies = glm::radians(10.0f);

    // La potencia cambia 0.25 por segundo.
    const float velocidadPotencia = 0.25f;

    // Elevador positivo: tendencia a bajar la nariz.
    controles_.elevator +=
        eje(GLFW_KEY_UP, GLFW_KEY_DOWN) *
        velocidadSuperficies * dt;

    // Aleron positivo: ala derecha hacia abajo.
    controles_.aileron +=
        eje(GLFW_KEY_RIGHT, GLFW_KEY_LEFT) *
        velocidadSuperficies * dt;

    // Timon negativo: nariz hacia la derecha.
    controles_.rudder +=
        eje(GLFW_KEY_A, GLFW_KEY_D) *
        velocidadSuperficies * dt;

    controles_.throttle +=
        eje(GLFW_KEY_W, GLFW_KEY_S) *
        velocidadPotencia * dt;

    // Respetamos los limites del modelo fisico.
    controles_.elevator = std::clamp(
        controles_.elevator,
        minimoElevador_,
        maximoElevador_
    );

    controles_.aileron = std::clamp(
        controles_.aileron,
        minimoAleron_,
        maximoAleron_
    );

    controles_.rudder = std::clamp(
        controles_.rudder,
        -maximoTimon_,
        maximoTimon_
    );

    controles_.throttle = std::clamp(
        controles_.throttle,
        0.0f,
        1.0f
    );
}