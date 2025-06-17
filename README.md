# Pixel Retro 3D Game Framework (pr3d)

**\*pr3d is in very early development. Breaking changes will happen regularly.**

pr3d is a code driven framework for making 3D games in the style of 90s retro consoles like the PS1, built from the ground up to natively render clean, chunky pixelated 3D graphics to modern displays.

pr3d is about making it simple to get a retro style 3D game project off the ground to get straight to coding gameplay and systems that matter.

pr3d is licensed under the [zlib License](https://zlib.net/zlib_license.html)

## Goals

pr3d aims to provide these features at a minimum:

### Rendering
* Pixel perfect fullscreen scaling from preset base resolutions similar to 90s retro games, but supporting modern 16:9 aspect ratios
* Native vertex wobble
* Affine texture mapping (warped textures)
* Flat and Gouraud shading
* glTF 3D model loading

### Input
* Keyboard and controller support

This list will be updated with time, right now the focus is on getting basic features in before thinking too far ahead. As more features are fully or at least mostly completed I will add a features section when it makes sense.

## Installation

At this point it is not recommended to start development using the framework, but if you are interested in how it is being developed or would like to fork the project for something you are working on you are more than welcome to. You can install pr3d on Linux systems very simply with the below steps:

```
git clone git@github.com:hyperchomp/pr3d.git
cd pr3d
mkdir build-debug
CC=clang cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON ..
make
sudo make install
```

Your Linux distribution will probably default to `/usr/local/` which is not normally a system include directory by default. If you want to instead install to the system directory you may add the cmake flag `-DCMAKE_INSTALL_PREFIX=/usr`. You probably don't have to use clang, but I do so there it is.

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
* [GLFW](https://github.com/glfw/glfw) for a multitude of functionality, for example rendering and input. pr3d wraps a lot of GLFW functionality so you shouldn't normally need to call it directly.
    * Most package managers have GLFW so you don't usually need to build it from source.
      * Fedora
        ```
        dnf install glfw glfw-devel
        ```
      * Arch
        ```
        pacman -S glfw
        ```
      * Ubuntu
        ```
        apt install libglfw3 libglfw3-dev
        ```
* [cglm](https://github.com/recp/cglm) For linear algebra (2D/3D math). This is what pr3d leverages for vectors, matrices, and all the math that goes along with them. pr3d is designed to be used alongside cglm so this one will need to be linked to your project.
    * cglm usually needs to be built from source, but it's very simple.
        ```
        git clone git@github.com:recp/cglm.git
        cd cglm
        cmake -DBUILD_SHARED_LIBS=ON  .
        make
        sudo make install
        ```
        As noted above in Installation, if you want to install to the system directory you may add the cmake flag `-DCMAKE_INSTALL_PREFIX=/usr`.

#### Linking
You can easily link pr3d and the required dependencies to your project in CMakeLists.txt. Note that for GLFW although you need it installed you don't need to link it as pr3d aims to provide similar functionality.
```
find_package(pr3d REQUIRED)
find_package(cglm REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pr3d cglm)
```

### Bundled
These dependencies are bundled in the project source code and don't require installation.
* [Glad](https://github.com/Dav1dde/glad) for **glad1** as a GL loader.
* [stb](https://github.com/nothings/stb) for stb_image as a loader for things like textures.

## Contributing

As of right now I'm not looking for contributors. This is a passion project of mine which I'm making for my own games and I want to maintain control over it. If I ever decide to take contributions I will update this section accordingly.

pr3d is open source under the zlib license and as such you are more than welcome to fork the project and use it for your own devilry as you see fit under the minimal stipulations of the license.
