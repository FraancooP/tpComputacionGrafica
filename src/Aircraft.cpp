#include "Aircraft.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <stdexcept>

namespace
{
    // Construye T * Rx * S.
    // Sobre los vertices se aplica: escala, rotacion, traslacion.
    glm::mat4 crearLocal(
        const glm::vec3 &posicion,
        const glm::vec3 &escala,
        float giroXGrados = 0.0f)
    {
        glm::mat4 matriz = glm::mat4(1.0f);

        matriz = glm::translate(matriz, posicion);

        matriz = glm::rotate(
            matriz,
            glm::radians(giroXGrados),
            glm::vec3(1.0f, 0.0f, 0.0f));

        matriz = glm::scale(matriz, escala);

        return matriz;
    }
}

void Aircraft::init(float L)
{
    if (!std::isfinite(L) || L <= 0.0f)
    {
        throw std::invalid_argument(
            "El largo del fuselaje debe ser positivo y finito.");
    }

    // Tres mallas para las seis piezas.
    cubo_.load(primitives::cube());
    cilindro_.load(primitives::cylinder(1.0f, 1.0f, 32U));
    cono_.load(primitives::cone(1.0f, 90.0f, 32U));

    // Medidas provisionales del despiece.
    const float largoNariz = L / 5.0f;
    const float radioNariz = L / 10.0f;

    const float largoFuselaje = L;
    const float radioFuselaje = radioNariz;

    const float largoAla = L / 2.0f;
    const float cuerdaAla = L / 4.0f;
    const float espesorAla = L / 40.0f;

    const float posicionZAla =
        largoNariz + largoFuselaje * 0.45f;

    const float anchoEstabilizador = L / 2.0f;
    const float cuerdaEstabilizador = L / 5.0f;
    const float espesorEstabilizador = L / 40.0f;

    const float alturaDeriva = L / 3.0f;
    const float cuerdaDeriva = L / 5.0f;
    const float espesorDeriva = L / 40.0f;

    const float posicionZCola =
        largoNariz + largoFuselaje * 0.88f;

    // 0. Nariz.
    piezas_[0] = PiezaAvion{
        TipoMalla::Cono,
        crearLocal(
            glm::vec3(0.0f, 0.0f, largoNariz * 0.5f),
            glm::vec3(radioNariz, largoNariz, radioNariz),
            -90.0f),
        glm::vec3(1.0f, 0.5f, 0.2f)};

    // 1. Fuselaje.
    piezas_[1] = PiezaAvion{
        TipoMalla::Cilindro,
        crearLocal(
            glm::vec3(
                0.0f,
                0.0f,
                largoNariz + largoFuselaje * 0.5f),
            glm::vec3(
                radioFuselaje,
                largoFuselaje,
                radioFuselaje),
            -90.0f),
        glm::vec3(0.7f, 0.75f, 0.8f)};

    // 2. Ala izquierda.
    piezas_[2] = PiezaAvion{
        TipoMalla::Cubo,
        crearLocal(
            glm::vec3(-largoAla * 0.5f, 0.0f, posicionZAla),
            glm::vec3(largoAla, espesorAla, cuerdaAla)),
        glm::vec3(0.25f, 0.55f, 0.9f)};

    // 3. Ala derecha.
    piezas_[3] = PiezaAvion{
        TipoMalla::Cubo,
        crearLocal(
            glm::vec3(largoAla * 0.5f, 0.0f, posicionZAla),
            glm::vec3(largoAla, espesorAla, cuerdaAla)),
        glm::vec3(0.25f, 0.55f, 0.9f)};

    // 4. Estabilizador horizontal.
    piezas_[4] = PiezaAvion{
        TipoMalla::Cubo,
        crearLocal(
            glm::vec3(0.0f, 0.0f, posicionZCola),
            glm::vec3(
                anchoEstabilizador,
                espesorEstabilizador,
                cuerdaEstabilizador)),
        glm::vec3(0.3f, 0.8f, 0.45f)};

    // 5. Deriva vertical.
    piezas_[5] = PiezaAvion{
        TipoMalla::Cubo,
        crearLocal(
            glm::vec3(
                0.0f,
                alturaDeriva * 0.5f,
                posicionZCola),
            glm::vec3(
                espesorDeriva,
                alturaDeriva,
                cuerdaDeriva)),
        glm::vec3(0.95f, 0.8f, 0.2f)};

    // 6. Cono adicional de cola.
    // Conservamos las medidas y la orientacion de tu modelo.
    const float largoCola = L / 5.0f;
    const float radioCola = radioNariz;

    piezas_[6] = PiezaAvion{
        TipoMalla::Cono,
        crearLocal(
            glm::vec3(
                0.0f,
                0.0f,
                posicionZCola + largoCola * 0.5f),
            glm::vec3(
                radioCola,
                largoCola,
                radioCola),
            -90.0f),
        glm::vec3(0.5f, 0.5f, 0.5f)};

    // Referencia de prueba: centro del tramo cilindrico.
    // No representa un centro de gravedad calculado.
    puntoReferencia_ = glm::vec3(
        0.0f,
        0.0f,
        largoNariz + largoFuselaje * 0.5f);
}

const Mesh &Aircraft::malla(TipoMalla tipo) const
{
    switch (tipo)
    {
    case TipoMalla::Cubo:
        return cubo_;

    case TipoMalla::Cilindro:
        return cilindro_;

    case TipoMalla::Cono:
        return cono_;
    }

    throw std::invalid_argument("Tipo de malla desconocido.");
}