# Just Another Code Editor (JACE)
JACE, short for Just Another Code Editor, is a open-source text editor design to write code while being stable and light-weight on performance.

## Features
- Lightweight: Made in C++, JACE uses less system resources then other editors
- Modular: Built from the ground up with a Plugin System and with many setting giving maximum control to the user
- Cross-platform: Designed to run on both Windows and Linux operating systems (May run on MacOS, but without one, this is unkown)

## Instalation
Pre-commpiled versions can be downloaded for various operating systems here[LINK, TODO].

### Building from Source
If you wish to build JACE from source instead, have the following installed and run CMake using the provided CMakelists.txt

#### Prerequisites (Linux Only)
- X11 Development Package (libx11-dev on Debian12), **Note the X11 package is only required if 'UNIX_DYNAMIC_DS' is set to 'TRUE' in the 'CMakeLists.txt' file.**
- Wayland Development Package (libwayland-dev on Debian12)

## Contribution
[TODO]


## License
JACE is licensed under the MIT License, see [LICENSE.txt](https://github.com/Hedge239/JustAnotherCodeEditor/blob/latest-dev/LICENSE.txt) for more information
> Copyright (c) 2023-2024 Lucas I Kohorst
