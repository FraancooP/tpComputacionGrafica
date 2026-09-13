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
            // Cara frontal: normal +Z.
            {{-x, -y,  z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ x, -y,  z}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ x,  y,  z}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-x,  y,  z}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

            // Cara posterior: normal -Z.
            {{ x, -y, -z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{-x, -y, -z}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-x,  y, -z}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{ x,  y, -z}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

            // Cara derecha: normal +X.
            {{ x, -y,  z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{ x, -y, -z}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{ x,  y, -z}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{ x,  y,  z}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

            // Cara izquierda: normal -X.
            {{-x, -y, -z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-x, -y,  z}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-x,  y,  z}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{-x,  y, -z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

            // Cara superior: normal +Y.
            {{-x,  y,  z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ x,  y,  z}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ x,  y, -z}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-x,  y, -z}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

            // Cara inferior: normal -Y.
            {{-x, -y, -z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ x, -y, -z}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ x, -y,  z}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-x, -y,  z}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}
        };

        datos.indices = {
             0,  1,  2,     0,  2,  3,
             4,  5,  6,     4,  6,  7,
             8,  9, 10,     8, 10, 11,
            12, 13, 14,    12, 14, 15,
            16, 17, 18,    16, 18, 19,
            20, 21, 22,    20, 22, 23
        };

        return datos;
    }
}