# Pixel Retro 3D Game Framework (pr3d)

pr3d is in early stages of development and barely anything right now, but the goal is to be a code driven framework for making 3D games in the style of 90s retro consoles like the PS1, but without the limitations of the hardware and with support for scaling to modern resolutions in mind from the start.

pr3d is built on top of the [GLFW library](https://github.com/glfw/glfw)

pr3d is licensed under the [zlib License](https://zlib.net/zlib_license.html)

## Foundations

pr3d aims to provide a retro 3D style by providing these foundations:

* Simple fullscreen scaling from preset base resolution
  * No need for tracking scale variables as the game will be rendered in a low resolution
* 16/32-bit fixed point graphics calculations
  * Native vertex wobble and other side effects
  * PS1 implementation uses 32-bit for world coordinates and 16-bit for local
* Affine texture mapping
* Flat and Gouraud shading
