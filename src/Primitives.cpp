#include "Primitives.h"

namespace primitives
{
    MeshData cube(
        float scale_x,
        float scale_y,
        float scale_z
    )
    {
        MeshData datos;

        const float x = scale_x * 0.5f;
        const float y = scale_y * 0.5f;
        const float z = scale_z * 0.5f;

        datos.vertices = {
            // Cara frontal: +Z. Rojo.
            // Vertices 0 a 3.
            {-x, -y,  z,    1.0f, 0.0f, 0.0f},
            { x, -y,  z,    1.0f, 0.0f, 0.0f},
            { x,  y,  z,    1.0f, 0.0f, 0.0f},
            {-x,  y,  z,    1.0f, 0.0f, 0.0f},

            // Cara posterior: -Z. Verde.
            // Vertices 4 a 7.
            { x, -y, -z,    0.0f, 1.0f, 0.0f},
            {-x, -y, -z,    0.0f, 1.0f, 0.0f},
            {-x,  y, -z,    0.0f, 1.0f, 0.0f},
            { x,  y, -z,    0.0f, 1.0f, 0.0f},

            // Cara derecha: +X. Azul.
            // Vertices 8 a 11.
            { x, -y,  z,    0.0f, 0.0f, 1.0f},
            { x, -y, -z,    0.0f, 0.0f, 1.0f},
            { x,  y, -z,    0.0f, 0.0f, 1.0f},
            { x,  y,  z,    0.0f, 0.0f, 1.0f},

            // Cara izquierda: -X. Amarillo.
            // Vertices 12 a 15.
            {-x, -y, -z,    1.0f, 1.0f, 0.0f},
            {-x, -y,  z,    1.0f, 1.0f, 0.0f},
            {-x,  y,  z,    1.0f, 1.0f, 0.0f},
            {-x,  y, -z,    1.0f, 1.0f, 0.0f},

            // Cara superior: +Y. Magenta.
            // Vertices 16 a 19.
            {-x,  y,  z,    1.0f, 0.0f, 1.0f},
            { x,  y,  z,    1.0f, 0.0f, 1.0f},
            { x,  y, -z,    1.0f, 0.0f, 1.0f},
            {-x,  y, -z,    1.0f, 0.0f, 1.0f},

            // Cara inferior: -Y. Cian.
            // Vertices 20 a 23.
            {-x, -y, -z,    0.0f, 1.0f, 1.0f},
            { x, -y, -z,    0.0f, 1.0f, 1.0f},
            { x, -y,  z,    0.0f, 1.0f, 1.0f},
            {-x, -y,  z,    0.0f, 1.0f, 1.0f}
        };

        datos.indices = {
             0,  1,  2,     0,  2,  3,  // Frontal
             4,  5,  6,     4,  6,  7,  // Posterior
             8,  9, 10,     8, 10, 11,  // Derecha
            12, 13, 14,    12, 14, 15,  // Izquierda
            16, 17, 18,    16, 18, 19,  // Superior
            20, 21, 22,    20, 22, 23   // Inferior
        };

        return datos;
    }
}