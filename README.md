# dice
3D dice simulator

## How to build

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

3. TBD

## TODOs

- [ ] Options to select shader precisions in web (highp, mediump, lowp)
- [ ] Simplify the normal matrix calculation ([link](https://lxjk.github.io/2017/10/01/Stop-Using-Normal-Matrix.html))
- [ ] Prevent re-calculate model matrix in Mesh
- [ ] model load
- [ ] keyboard input
- [ ] reduce pkg size with replacing the big packages (like assimp)
