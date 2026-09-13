#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

ResourceManager::ResourceManager(
    const std::filesystem::path& assets_root
)
    : assets_root_(assets_root)
{
}

std::string ResourceManager::read_shader_file(
    const std::string& shader_file,
    const std::string& shader_type
)
{
    const std::filesystem::path ruta =
        assets_root_ / shader_file;

    std::ifstream archivo(ruta);

    if (!archivo.is_open())
    {
        throw std::runtime_error(
            "No se pudo abrir el " + shader_type +
            ": " + ruta.string()
        );
    }

    std::cout
        << "[ResourceManager] Leyendo: "
        << ruta.string()
        << '\n';

    std::ostringstream contenido;
    contenido << archivo.rdbuf();

    if (archivo.bad())
    {
        throw std::runtime_error(
            "Error al leer el " + shader_type +
            ": " + ruta.string()
        );
    }

    return contenido.str();
}

const ShaderSource& ResourceManager::load_shader_source(
    const std::string& key,
    const std::string& vs_file,
    const std::string& fs_file,
    const std::string& gs_file
)
{
    const auto encontrado = shaders_sources_.find(key);

    if (encontrado != shaders_sources_.end())
    {
        return encontrado->second;
    }

    ShaderSource fuentes;

    fuentes.vs = read_shader_file(
        vs_file,
        "vertex shader"
    );

    fuentes.fs = read_shader_file(
        fs_file,
        "fragment shader"
    );

    if (!gs_file.empty())
    {
        fuentes.gs = read_shader_file(
            gs_file,
            "geometry shader"
        );
    }

    const auto resultado = shaders_sources_.emplace(
        key,
        std::move(fuentes)
    );

    return resultado.first->second;
}

const ShaderSource& ResourceManager::get_shader_source(
    const std::string& key
) const
{
    const auto encontrado = shaders_sources_.find(key);

    if (encontrado == shaders_sources_.end())
    {
        throw std::runtime_error(
            "No hay shaders cargados con la clave: " + key
        );
    }

    return encontrado->second;
}

void ResourceManager::clear()
{
    shaders_sources_.clear();
}