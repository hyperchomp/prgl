# Pixel Retro 3D Game Framework (pr3d)

## What is pr3d?

**pr3d is made for me, by me first and foremost. I have made the code open source so anyone can re-use it, analyze it, and benefit from it but above all things it is made by me to make my own games. It is not a one size fits all framework and it does not aim to be.**

pr3d is a code driven framework for making 3D games in a retro style similar to games made in the 80s and 90s, built from the ground up to natively render clean, chunky pixelated 3D graphics to modern displays.

## Licensing

pr3d is licensed under the [zlib License](https://zlib.net/zlib_license.html)

Libraries used within pr3d may have differing licenses that are useable alongside the zlib license. Licenses for dependencies are listed below in the Dependencies section.

## Current Features

### Game
* Fly camera supporting directional movement and rotation in pitch/yaw 

### Rendering
* Simple 3D meshes such as triangles and cubes
* Textured rectangles and cubes
* Gouraud shading
* Point lights
* Custom shaders
* 2D Rendering for basic shapes (GUI)

### Input
* Keyboard/Mouse

## Goal Features

### Game
* FPS camera
* Box collisions

### Rendering
* Pixel perfect fullscreen scaling from preset base resolution(s) similar to 90s retro games, but supporting modern 16:9 aspect ratios
* glTF 3D model loading
* Text rendering

### Input
* Gamepad support

This list will be updated with time, right now the focus is on getting basic features in before thinking too far ahead.

## Installation

At this point it is not recommended to start development using the framework, but if you are interested in how it is being developed or would like to fork the project for something you are working on you are more than welcome to. You can install pr3d on Linux systems very simply with the below steps:

```
git clone git@github.com:hyperchomp/pr3d.git
cd pr3d
mkdir build-debug
CC=clang cmake -DCMAKE_BUILD_TYPE=Debug ..
make
sudo make install
```

You don't strictly need to compile it with clang, but that's how I build it during development so I can't guarantee it will work with other compilers - although I'd expect it should if it's a well known compiler like gcc.

After running the above if you need to rebuild and haven't cleaned or deleted the build directory you can simply do the following from within the build directory:

```
cmake ..
make
sudo make install
```

After this (and the dependencies below!) simply include any necessary pr3d modules into your project. They are in a subfolder structure so you should include them like so:

```
#include <pr3d/game.h>
#include <pr3d/screen.h>
// etc...
```

## Dependencies
### Install
These dependencies must be installed separately from pr3d as the framework relies on them.
* [GLFW](https://github.com/glfw/glfw) (zlib) for a multitude of functionality, for example rendering and input. pr3d wraps a lot of GLFW functionality so you shouldn't normally need to call it directly.
    * I recommend building GLFW from source as a static library, this process is very similar to installing pr3d. The directions can be found [in the GLFW documentation](https://www.glfw.org/docs/latest/compile.html).

#### Linking
You can easily link pr3d to your project in CMakeLists.txt. Note that for GLFW although you need it installed to compile pr3d itself you aren't required to link it in another project using the framework as pr3d aims to provide wrapper methods for any needed GLFW functionality.
```
find_package(pr3d REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pr3d)
```

### Bundled
These dependencies are bundled in the project source code and don't require installation.
* [Glad](https://github.com/Dav1dde/glad) (MIT) for **glad1** as a GL loader.
* [stb](https://github.com/nothings/stb) (MIT) for stb_image as a loader for things like textures.
* [cglm](https://github.com/recp/cglm) (MIT) For linear algebra (2D/3D math). This is what pr3d leverages for vectors, matrices, and all the math that goes along with them. This library is meant to be used directly when developing with pr3d for the data structures and supporting math it provides.

## Contributing

As of right now I'm not looking for contributors. This is a passion project of mine which I'm making for my own games and I want to maintain control over it. If I ever decide to take contributions I will update this section accordingly.

pr3d is open source under the zlib license and as such you are more than welcome to fork the project and use it for your own devilry as you see fit under the minimal stipulations of the license.
