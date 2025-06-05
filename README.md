# Pixel Retro 3D Game Framework (pr3d)

**\*pr3d is in very early development. Breaking changes will happen regularly.**

pr3d is a code driven framework for making 3D games in the style of 90s retro consoles like the PS1, built from the ground up to natively render clean, chunky pixelated 3D graphics to modern displays.

pr3d is about making it as simple as possible to get a retro style 3D game project off the ground so you can get straight to coding the gameplay.

pr3d is licensed under the [zlib License](https://zlib.net/zlib_license.html)

## Goals

pr3d aims to provide these features:

* Pixel perfect fullscreen scaling from preset base resolutions similar to 90s retro games, but supporting modern 16:9 aspect ratios
* Native vertex wobble
* Affine texture mapping
* Flat and Gouraud shading
* Keyboard and controller support
* 

This list will be updated with time, right now the focus is on getting basic features in before thinking too far ahead. As more features are fully or at least mostly completed I will add a features section when it makes sense.

## Dependencies

* [GLFW](https://github.com/glfw/glfw) for a multitude of functionality, for example rendering and input.
* [Glad](https://github.com/Dav1dde/glad) for **glad1** as a GL loader, the repo already contains glad.c exported with GL version 4.6 (all extensions exported) but you will need to add glad.h to one of your system include folders like `/usr/local/include` or wherever pleases you.

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

If your Linux distribution defaults to something like `/usr/local/` and you want to instead install to the system directory **at your own risk** you may add the cmake flag `-DCMAKE_INSTALL_PREFIX=/usr`. You probably don't have to use clang, but I do so there it is.

After running the above if you need to rebuild and haven't cleaned or deleted the build directory you can simply do the following from within the build directory:

```
cmake ..
make
sudo make install
```

After this simply include any necessary pr3d modules into your project. They are in a subfolder structure so you should include them like so:

```
#include <pr3d/window.h>
```

## Contributing

As of right now I'm not looking for contributors to the project. It is is a passion project of mine, and I would like to maintain control over it. If I ever decide to take contributions I will update this section accordingly.

pr3d is open source under the zlib license and as such you are more than welcome to fork the project and use it for your own devilry as you see fit under the minimal stipulations of the license.
