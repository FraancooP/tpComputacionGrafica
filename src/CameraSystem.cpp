#include "CameraSystem.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

CameraSystem::CameraSystem(int width, int height)
{
    set_viewport(width, height);

    update(
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        CameraCommand{}
    );
}

void CameraSystem::set_viewport(int width, int height)
{
    // Al minimizar, alguna dimension puede ser cero.
    // Conservamos la proyeccion anterior hasta tener un tamaño valido.
    if (width <= 0 || height <= 0)
    {
        return;
    }

    const float aspect =
        static_cast<float>(width) /
        static_cast<float>(height);

    data_.projection = glm::perspective(
        glm::radians(45.0f),
        aspect,
        0.1f,
        100.0f
    );
}

void CameraSystem::update(
    const glm::vec3& objetivo,
    const glm::vec3& angulosAvion,
    const CameraCommand& comando
)
{
    // La camara orbital sigue la posicion del objetivo.
    // Su orientacion no depende de los angulos del avion.
    (void)angulosAvion;

    yaw_ += comando.yaw_delta;
    pitch_ += comando.pitch_delta;
    distancia_ += comando.dist_delta;

    // Evitamos que el angulo crezca indefinidamente.
    yaw_ = std::remainder(
        yaw_,
        glm::two_pi<float>()
    );

    // No llegamos a mirar exactamente desde arriba o abajo.
    const float limitePitch = glm::radians(81.0f);

    pitch_ = std::clamp(
        pitch_,
        -limitePitch,
        limitePitch
    );

    distancia_ = std::clamp(
        distancia_,
        distanciaMinima_,
        distanciaMaxima_
    );

    const float cosPitch = std::cos(pitch_);

    const glm::vec3 desplazamiento(
        distancia_ * cosPitch * std::cos(yaw_),
        distancia_ * std::sin(pitch_),
        distancia_ * cosPitch * std::sin(yaw_)
    );

    const glm::vec3 eye = objetivo + desplazamiento;

    data_.view = glm::lookAt(
        eye,
        objetivo,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}