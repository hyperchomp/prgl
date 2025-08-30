# Pixel Retro GL Framework

![GitHub Release](https://img.shields.io/github/v/release/hyperchomp/prgl?include_prereleases&label=prgl)

**prgl is a personal project created to build my own games. I've made the code open source so anyone can re-use, analyze, and/or build their own projects with it.

https://github.com/user-attachments/assets/2dea8636-c0ae-4bc7-9748-15f0225971ee

prgl is a code driven framework for making 3D games in a retro style similar to games made in the 80s and 90s, built from the ground up to natively render clean, chunky pixelated 3D graphics to modern displays.

## Licensing

prgl is licensed under the [zlib License](https://zlib.net/zlib_license.html)

Libraries used within prgl may have differing licenses that are useable alongside the zlib license. Licenses for dependencies are listed below in the Dependencies section.

## Current Features

### Game
* Fly camera supporting directional movement and rotation in pitch/yaw 

### Rendering
* Pixel perfect fullscreen scaling from base resolution similar to 90s retro games, but supporting modern 16:9 aspect ratios
* Gouraud shading
* Pixel wobble/jitter
* Simple 3D meshes such as triangles and cubes
* Textured rectangles and cubes
* Point lights
* Custom shaders
* 2D Rendering for basic shapes (GUI)

### Input
* Keyboard/Mouse

## Goal Features

### Game
* Configurable Camera Modes (FPS, Third Person, top down, fixed)
* Box collisions

### Rendering
* glTF 3D model loading
* Text rendering
* Fog
* Billboard sprites

### Input
* Gamepad support

### Sound
* Audio loading and playback

## Getting Started

If you want to dive into the code or adapt it for your own project you are more than welcome to. You can install prgl on Linux systems very simply with the below steps:

### Method 1: System level install (requires sudo)

Use this method if you want to install prgl to the default system path, usually something like `/usr/local/`

```sh
git clone git@github.com:hyperchomp/prgl.git
cd prgl
mkdir build-debug
CC=clang cmake -DCMAKE_BUILD_TYPE=Debug ..
make
sudo make install
```

Note: I use clang for development so I include that in the commands, but I'd expect it should work with other well known compilers like gcc.

### Method 2: User local installation (no sudo)

If you'd rather install somewhere like the home directory where root is not required you can use this method.

```sh
git clone git@github.com:hyperchomp/prgl.git
cd prgl
mkdir build-debug
cd build-debug
CC=clang cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$HOME/.local ..
make
make install
```

#### Finding a local installation with CMake

Because prgl's CMake generates a compile_commands.json file, your chosen IDE or LSP compatible editor should automatically detect the correct include paths.

For your own project's CMakeLists.txt to find prgl, simply set CMAKE_PREFIX_PATH when running the cmake command:

```sh
cmake -DCMAKE_PREFIX_PATH=$HOME/.local ..
```

### Rebuilding

To rebuild and reinstall you can go back into your build directory and run:

```sh
cmake ..
make
# System install
sudo make install
# User install
make install
```

### Usage

After installing simply include any necessary prgl modules into your project like so:

```C
#include <prgl/game.h>
#include <prgl/screen.h>
// etc...
```

## Dependencies
### Install
These dependencies must be installed separately as the framework relies on them.
* [GLFW](https://github.com/glfw/glfw) (zlib) for a multitude of functionality, for example rendering and input. prgl wraps a lot of GLFW functionality so you shouldn't normally need to call it directly.
    * I recommend building GLFW from source as a static framework. The directions can be found [in the GLFW documentation](https://www.glfw.org/docs/latest/compile.html).

#### Linking
You can easily link prgl to your project in your `CMakeLists.txt`. Note that although you need GLFW installed to compile prgl you aren't required to link it in a project using the framework as prgl provides wrapper methods for needed GLFW functionality.
```
find_package(prgl REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE prgl)
```

### Bundled
These dependencies are bundled in the project source code and don't require separate installation.
* [Glad](https://github.com/Dav1dde/glad) (MIT) for **glad1** as a GL loader.
* [stb](https://github.com/nothings/stb) (MIT) for stb_image as a loader for things like textures.
* [cglm](https://github.com/recp/cglm) (MIT) For linear algebra (2D/3D math). This is what prgl leverages for vectors, matrices, and all the math that goes along with them. This framework is meant to be used directly when developing with prgl for the data structures and supporting math it provides.
