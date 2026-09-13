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
        float scale_z = 1.0f
    );
}