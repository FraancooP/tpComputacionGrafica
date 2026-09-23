#pragma once

struct CameraCommand
{
    // Cambios angulares, expresados en radianes.
    float yaw_delta = 0.0f;
    float pitch_delta = 0.0f;

    // Cambio de distancia en unidades de la escena.
    // Positivo: alejarse del objetivo.
    float dist_delta = 0.0f;
};