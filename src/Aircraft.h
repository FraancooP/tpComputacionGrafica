#pragma once

#include "Mesh.h"

#include <glm/glm.hpp>

#include <array>

enum class TipoMalla
{
    Cubo,
    Cilindro,
    Cono
};

struct PiezaAvion
{
    TipoMalla tipo = TipoMalla::Cubo;
    glm::mat4 local = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f);
};

class Aircraft
{
public:
    // Requiere un contexto de OpenGL activo.
    // L representa el largo del tramo cilindrico.
    void init(float L = 1.0f);

    const std::array<PiezaAvion, 7>& piezas() const
    {
        return piezas_;
    }

    const Mesh& malla(TipoMalla tipo) const;

    glm::vec3 puntoReferencia() const
    {
        return puntoReferencia_;
    }

private:
    Mesh cubo_;
    Mesh cilindro_;
    Mesh cono_;

    std::array<PiezaAvion, 7> piezas_;

    glm::vec3 puntoReferencia_ = glm::vec3(0.0f);
};