# dice

> **NOTICE**: This project is suspended and archived due to a lack of solid project design. 📦

- Interactive 3D dice simulator for various platforms.

## Tech Stack

- Assimp (model file loader)
- Bullet Physics (physics engine)
- GLFW (OpenGL helper on desktop)
- GLAD (OpenGL linker on desktop)
- Emscripten (WASM compiler)
- Vite (Web build tool)

## How to build

### Desktop

1. Clone git repo
    - You must include submodules with `--recursive` tag when you clone this repo

    ```zsh
    git clone https://github.com/ColorOfLight/dice --recursive
    git submodule update --init --recursive
    ```

2. Move to `cpps` directory and create Makefile with CMake preset

    ```zsh
    cd cpps

    # If you want build for desktop,
    cmake . --preset=glfw
    cd build-glfw
    ninja
    bin/DiceProject

    # If you want build for web,
    cmake . --preset=emscripten
    ninja
    # After compilation, use .js and .wasm file
    ```

### Web

- In this case, you don't need to include submodules

    ```zsh
    # Clone this repo
    git clone https://github.com/ColorOfLight/dice

    # Move to web directory
    cd web

    # Install dependencies
    npm install

    # Run dev server
    npm run dev
    ```
