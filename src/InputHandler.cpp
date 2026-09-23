#include "InputHandler.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

CameraCommand InputHandler::update(GLFWwindow* ventana)
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
