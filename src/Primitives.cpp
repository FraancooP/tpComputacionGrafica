#include "Primitives.h"
#include <glm/gtc/constants.hpp>

#include <cmath>
#include <stdexcept>

namespace primitives
{
    MeshData cube(
        float scale_x,
        float scale_y,
        float scale_z)
    {
        MeshData datos;

        const float x = scale_x * 0.5f;
        const float y = scale_y * 0.5f;
        const float z = scale_z * 0.5f;

        datos.vertices = {
            // Cara frontal: normal +Z.
            {{-x, -y, z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{x, -y, z}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{x, y, z}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-x, y, z}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

            // Cara posterior: normal -Z.
            {{x, -y, -z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{-x, -y, -z}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-x, y, -z}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{x, y, -z}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

            // Cara derecha: normal +X.
            {{x, -y, z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{x, -y, -z}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{x, y, -z}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{x, y, z}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

            // Cara izquierda: normal -X.
            {{-x, -y, -z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-x, -y, z}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-x, y, z}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{-x, y, -z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

            // Cara superior: normal +Y.
            {{-x, y, z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{x, y, z}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{x, y, -z}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-x, y, -z}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

            // Cara inferior: normal -Y.
            {{-x, -y, -z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{x, -y, -z}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{x, -y, z}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-x, -y, z}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}};

        datos.indices = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23};

        return datos;
    }

    MeshData cylinder(
        float radio,
        float largo,
        unsigned int gajos,
        unsigned int anillos)
    {
        if (radio <= 0.0f || largo <= 0.0f ||
            gajos < 3U || anillos < 1U)
        {
            throw std::invalid_argument(
                "Cilindro: radio y largo positivos, "
                "al menos 3 gajos y 1 tramo longitudinal.");
        }

        MeshData datos;

        const float vuelta = glm::two_pi<float>();
        const float mitadLargo = largo * 0.5f;

        const unsigned int porAnillo = gajos + 1U;

        // --------------------------------------------------
        // 1. Vertices del lateral.
        // --------------------------------------------------

        for (unsigned int i = 0; i <= anillos; ++i)
        {
            const float v =
                static_cast<float>(i) /
                static_cast<float>(anillos);

            const float y = -mitadLargo + largo * v;

            for (unsigned int j = 0; j <= gajos; ++j)
            {
                const float u =
                    static_cast<float>(j) /
                    static_cast<float>(gajos);

                // Cerramos la costura con la misma posicion exacta.
                const float angulo =
                    (j == gajos) ? 0.0f : vuelta * u;

                const float c = std::cos(angulo);
                const float s = std::sin(angulo);

                Vertex vertice;

                vertice.position = glm::vec3(
                    radio * c,
                    y,
                    radio * s);

                vertice.normal = glm::vec3(
                    c,
                    0.0f,
                    s);

                vertice.tex_coords = glm::vec2(u, v);

                datos.vertices.push_back(vertice);
            }
        }

        // --------------------------------------------------
        // 2. Indices del lateral.
        // --------------------------------------------------

        for (unsigned int i = 0; i < anillos; ++i)
        {
            for (unsigned int j = 0; j < gajos; ++j)
            {
                const unsigned int b0 = i * porAnillo + j;
                const unsigned int b1 = b0 + 1U;

                const unsigned int t0 = b0 + porAnillo;
                const unsigned int t1 = t0 + 1U;

                datos.indices.insert(
                    datos.indices.end(),
                    {b0, t1, b1,
                     b0, t0, t1});
            }
        }

        // --------------------------------------------------
        // 3. Tapas: inferior y superior.
        // --------------------------------------------------

        for (unsigned int tapa = 0; tapa < 2U; ++tapa)
        {
            const float signo = (tapa == 0U) ? -1.0f : 1.0f;
            const float y = signo * mitadLargo;

            const glm::vec3 normal(0.0f, signo, 0.0f);

            const unsigned int centro =
                static_cast<unsigned int>(datos.vertices.size());

            datos.vertices.push_back(
                Vertex{
                    glm::vec3(0.0f, y, 0.0f),
                    normal,
                    glm::vec2(0.5f, 0.5f)});

            const unsigned int inicioBorde =
                static_cast<unsigned int>(datos.vertices.size());

            // En las tapas no hace falta duplicar la costura.
            for (unsigned int j = 0; j < gajos; ++j)
            {
                const float angulo =
                    vuelta * static_cast<float>(j) /
                    static_cast<float>(gajos);

                const float c = std::cos(angulo);
                const float s = std::sin(angulo);

                datos.vertices.push_back(
                    Vertex{
                        glm::vec3(radio * c, y, radio * s),
                        normal,
                        glm::vec2(
                            0.5f + 0.5f * c,
                            0.5f + 0.5f * s)});
            }

            for (unsigned int j = 0; j < gajos; ++j)
            {
                const unsigned int actual = inicioBorde + j;

                const unsigned int siguiente =
                    inicioBorde + (j + 1U) % gajos;

                if (tapa == 0U)
                {
                    // Inferior: orientacion hacia -Y.
                    datos.indices.insert(
                        datos.indices.end(),
                        {centro, actual, siguiente});
                }
                else
                {
                    // Superior: orientacion hacia +Y.
                    datos.indices.insert(
                        datos.indices.end(),
                        {centro, siguiente, actual});
                }
            }
        }

        return datos;
    }
}