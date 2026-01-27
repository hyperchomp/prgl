# Pixel Retro GL Framework

![GitHub Release](https://img.shields.io/github/v/release/hyperchomp/prgl?include_prereleases&label=prgl)

**prgl is a personal project created to build my own games. I've made the code open source so anyone can re-use, analyze, and/or build their own projects with it.**

https://github.com/user-attachments/assets/2dea8636-c0ae-4bc7-9748-15f0225971ee

prgl is a code driven framework for making 3D games in a retro style similar to games made in the early 90s, built from the ground up to natively render clean, chunky pixelated 3D graphics to modern displays.

## Licensing

prgl is licensed under the [zlib License](https://zlib.net/zlib_license.html)

Libraries used within prgl may have differing licenses that are useable alongside the zlib license. Licenses for dependencies are listed below in the Dependencies section.

## Current Features

### Game
* Fly camera supporting directional movement and rotation in pitch/yaw 

### Rendering
* Automatic, fullscreen scaling from 320x180 to support modern resolutions with a retro aesthetic
* Gouraud shading/vertex lighting 
* Pixel wobble/jitter
* Primitives - Line Strips, Triangles, Quads, Circles, Cubes, Spheres, Pyramids
* Textured Meshes
* Point lights
* Custom shaders
* 2D Overlay Rendering

### Input
* Keyboard/Mouse

## Goal Features

### Game
* Configurable Camera Modes (FPS, Third Person, top down, fixed)
* Box collisions

### Rendering
* Support for 16:10 aspect ratio without stretching or black bars
* glTF 3D model loading
* Text rendering
* Fog
* Billboard sprites

### Input
* Gamepad support

### Sound
* Audio loading and playback

## Getting Started
### Windows

On Windows the recommended workflow is to use [Microsoft Visual Studio Community Edition](https://visualstudio.microsoft.com/vs/community/). This is mostly straight forward, but there are a few extra considerations below:

* During installation select the Workload **Desktop Development with C++** and in the Installation Details pane on the right make sure you have **C++ CMake tools for Windows** and **C++ Clang tools for Windows**
* Install GLFW as noted in the Dependencies section below.

### Linux

You can install prgl on Linux systems very simply with the below steps:

#### Method 1: System level install (requires sudo)

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

#### Method 2: User local installation (no sudo)

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

##### Finding a local installation with CMake

Because prgl's CMake generates a compile_commands.json file, your chosen IDE or LSP compatible editor should automatically detect the correct include paths.

For your own project's CMakeLists.txt to find prgl, simply set CMAKE_PREFIX_PATH when running the cmake command:

```sh
cmake -DCMAKE_PREFIX_PATH=$HOME/.local ..
```

#### Rebuilding

To rebuild and reinstall you can go back into your build directory and run:

```sh
cmake ..
make
# System install
sudo make install
# User install
make install
```

#### Usage

After installing and linking simply include any necessary prgl modules into your project like so:

```C
#include <prgl/game.h>
#include <prgl/screen.h>
// etc...
```

## Dependencies
### Install
These dependencies must be installed separately as the framework relies on them.
### [GLFW](https://github.com/glfw/glfw) (zlib) 
Required for a multitude of functionality, for example rendering and input. prgl wraps a lot of GLFW functionality so you shouldn't normally need to call it directly.

#### Windows
For installing with Windows using Visual Studio follow these steps. This outlines how to install GLFW as a static library (.lib file) on Windows.
    * There are [precompiled binaries here](https://www.glfw.org/download.html).
    * In the binaries zip there will be an include folder and a lib-vc20XX folder.
    * Create a folder somewhere on your PC for libraries, for example `C:/dev/libraries`.
    * In the above folder create a `GLFW` subfolder.
    * From the GLFW binaries zip `lib-vc20XX` folder copy the `glfw3.lib` file into the above GLFW subfolder.
    * From the GLFW binaries zip `include/GLFW` folder copy the header files and paste them into an `include/GLFW` subfolder in your top `GLFW` subfolder. This may seem redundant but this structure allows for easy, cross OS compatibility as some operating systems package managers install GLFW in this way.
       * The directory structure must be:
           `libraries/GLFW/glfw3.lib`
           `libraries/GLFW/include/GLFW/glfw3.h`
    * Update your project's `CMAKE_PREFIX_PATH` in `CMakeUserPresets.json` to point to the parent libraries folder. This is a local configuration file and it is not advised to commit this to any source control, it should look something like this:
```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "user-base",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "C:/dev/libraries"
      }
    },
    {
      "name": "x64-debug-user",
      "inherits": [ "x64-debug", "user-base" ]
    },
    {
      "name": "x64-release-user",
      "inherits": [ "x64-release", "user-base" ]
    }
  ]
}
```

#### Linux
I recommend building GLFW from source as a static framework. The directions can be found [in the GLFW documentation](https://www.glfw.org/docs/latest/compile.html).

### Bundled
These dependencies are bundled in the project source code and don't require separate installation.
* [Glad](https://github.com/Dav1dde/glad) (MIT) for **glad1** as a GL loader.
* [stb](https://github.com/nothings/stb) (MIT) for stb_image as a loader for things like textures.
* [cglm](https://github.com/recp/cglm) (MIT) For linear algebra (2D/3D math). This is what prgl leverages for vectors, matrices, and all the math that goes along with them. This framework is meant to be used directly when developing with prgl for the data structures and supporting math it provides.

## Linking
You can easily link prgl to your project in your `CMakeLists.txt`. Note that although you need GLFW installed to compile prgl you aren't required to link it in a project using the framework as prgl provides wrapper methods for needed GLFW functionality.
```cmake
find_package(prgl REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE prgl)
```
