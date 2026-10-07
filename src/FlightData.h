#pragma once

#include <glm/glm.hpp>
#include <dlfdm/defines.h>

// Posicion y orientacion expresadas en los ejes de nuestra escena.
struct FlightData
{
    glm::vec3 position{0.0f};

    float phi = 0.0f;    // Alabeo.
    float theta = 0.0f;  // Cabeceo.
    float psi = 0.0f;    // Guinada.
};

// Conversion indicada en la guia del TP6.
inline FlightData to_world(const dlfdm::AircraftState& ned)
{
    FlightData gl;

    gl.position.x = ned.inertial_position.y;
    gl.position.y = -ned.inertial_position.z;
    gl.position.z = -ned.inertial_position.x;

    gl.phi = -ned.phi;
    gl.theta = ned.theta;
    gl.psi = -ned.psi;

    return gl;
}