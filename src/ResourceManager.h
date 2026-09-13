#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

struct ShaderSource
{
    std::string vs;
    std::string fs;
    std::string gs;
};

class ResourceManager
{
public:
    explicit ResourceManager(
        const std::filesystem::path& assets_root
    );

    const ShaderSource& load_shader_source(
        const std::string& key,
        const std::string& vs_file,
        const std::string& fs_file,
        const std::string& gs_file = ""
    );

    const ShaderSource& get_shader_source(
        const std::string& key
    ) const;

    void clear();

private:
    std::filesystem::path assets_root_;

    std::unordered_map<std::string, ShaderSource> shaders_sources_;

    std::string read_shader_file(
        const std::string& shader_file,
        const std::string& shader_type
    );
};