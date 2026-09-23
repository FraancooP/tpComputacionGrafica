#pragma once

#include "CameraCommand.h"
#include "CameraData.h"

#include <glm/glm.hpp>

class CameraSystem
{
public:
    CameraSystem(int width, int height);

    void update(
        const glm::vec3& objetivo,
        const glm::vec3& angulosAvion,
        const CameraCommand& comando
    );

    void set_viewport(int width, int height);

    const CameraData& data() const
    {
        return data_;
    }

private:
    float yaw_ = glm::radians(-45.0f);
    float pitch_ = glm::radians(25.0f);

    // Valores elegidos para nuestro avion con L = 1.
    float distancia_ = 3.0f;
    float distanciaMinima_ = 1.2f;
    float distanciaMaxima_ = 20.0f;

    CameraData data_;
};