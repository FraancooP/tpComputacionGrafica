#pragma once

#include "MeshData.h"

namespace primitives
{
    // Cubo centrado en el origen.
    // Los parametros indican sus dimensiones completas en X, Y y Z.
    // Con los valores por defecto, cada lado mide 1.
    // Para dimensiones positivas, las caras tienen orden antihorario
    // vistas desde afuera.
    MeshData cube(
        float scale_x = 1.0f,
        float scale_y = 1.0f,
        float scale_z = 1.0f);

    // Cilindro cerrado, centrado en el origen y con eje longitudinal Y.
    // radio y largo deben ser positivos.
    // gajos: divisiones alrededor del eje, como minimo 3.
    // anillos: tramos longitudinales, como minimo 1.
    // Se generan anillos + 1 coronas de vertices en el lateral.
    MeshData cylinder(
        float radio,
        float largo,
        unsigned int gajos,
        unsigned int anillos = 1U);
}