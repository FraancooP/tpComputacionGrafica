#!/usr/bin/env bash
set -euo pipefail

# Trabajar desde la carpeta donde esta este script.
proyecto_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd -- "$proyecto_dir"

# Configurar.
cmake -S . -B build-linux \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Debug

# Compilar.
cmake --build build-linux --parallel 2

# Ejecutar.
exec ./build-linux/bin/cgyav