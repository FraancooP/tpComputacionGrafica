#pragma once

#include <string>

class Shader
{
public:
    Shader() = default;
    ~Shader();

    // El programa tiene un unico propietario: no se copia.
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Se permite transferir la propiedad a otro objeto.
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // Devuelve false si falla y deja el objeto vacio.
    bool compile_from_source(
        const std::string& vs,
        const std::string& fs
    );

    void use() const;

    // Libera el programa. Se puede llamar varias veces.
    void clear();

    unsigned int id() const
    {
        return id_;
    }

private:
    unsigned int id_ = 0;
};