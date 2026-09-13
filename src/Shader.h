#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader() = default;
    ~Shader();

    // El programa tiene un unico propietario: no se copia.
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    // Se permite transferir la propiedad a otro objeto.
    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    // Devuelve false si falla y deja el objeto vacio.
    bool compile_from_source(
        const std::string &vs,
        const std::string &fs);

    void use() const;

    // Consulta la ubicacion de un uniform del programa enlazado.
    int loc(const std::string &nombre) const;

    // Envia una matriz indicando el nombre del uniform.
    void set_uniform(
        const std::string &nombre,
        const glm::mat4 &matriz) const;

    // Envia una matriz utilizando una ubicacion ya consultada.
    void set_uniform(
        int ubicacion,
        const glm::mat4 &matriz) const;

    // Libera el programa. Se puede llamar varias veces.
    void clear();
        
    // Devuelve el ID del programa.
    unsigned int id() const
    {
        return id_;
    }

private:
    unsigned int id_ = 0;
};