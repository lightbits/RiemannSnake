Riemann Snake
============

About
------------
![Preview](https://raw.github.com/lightbits/riemannsnake/master/devlog/screenshot0.PNG)

Riemann Snake is a simple 3D game written using C++ and OpenGL.
Navigate the snake with the left and right arrow keys, and avoid the enemies.
Move over green apples to grow longer and increase your score!

The game is open source, with code available on [Github](https://github.com/lightbits/riemannsnake). 

Dependencies
------------
This project uses the following libraries:

*	GLFW 3 for OpenGL context, input and window
*	GLM for matrix mathematics
*	glload for loading OpenGL functions
*	irrKlang for audio
*	lodepng for loading image files

For details on building and linking these, see the associated pages at the GLSDK wiki for [building](http://glsdk.sourceforge.net/docs/html/pg_build.html) and [usage](http://glsdk.sourceforge.net/docs/html/pg_use.html).

lodepng can be included as a header file, irrKlang is linked dynamically and requires the .dll to be available.

Building for Windows
------------
The game has been built with VS2012, and requires some c++11 features.
To build for Windows:
*	Set up the required include and library directories
*	Specify dependencies: irrKlang.lib, glfw3.lib, glload(D).lib, opengl32.lib

You may need to include irrKlang.dll in the same folder as the executable.

Building for Linux
------------
The game has not yet been tested on Linux.

Licence and copyright
------------
Riemann Snake uses libraries which are part of the [Unofficial OpenGL Software Development Kit](http://glsdk.sourceforge.net/docs/html/index.html).
*	GLFW is under the [zlib/libpng](http://opensource.org/licenses/zlib-license.php) license 
*	GLM is under the [MIT license](http://opensource.org/licenses/MIT)
*	glload is under the [MIT license](http://opensource.org/licenses/MIT)

Additional libraries are under the licences given by their authors:
*	irrKlang
*	lodepng