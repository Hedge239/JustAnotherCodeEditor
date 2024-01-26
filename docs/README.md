# Just Another Code Editor (JACE)
JACE, short for Just Another Code Editor, is a open-source text editor design to write code while being stable and light-weight on performance.

## Features
- Lightweight: Made in C++, JACE uses less system resources then other editors
- Modular: Built from the ground up with a Plugin System and with many setting giving maximum control to the user
- Cross-platform: Designed to run on both Windows and Linux operating systems (May run on MacOS, but without one, this is unkown)

## Instalation
Pre-commpiled versions can be downloaded for various operating systems here[LINK, TODO].

### Building from Source
If you wish to build JACE from source instead, follow these steps:
1. Have the following packages installed
    - Windows
        - CMake
    - Linux (Debian-based)
        - libx11-dev    (Optional: have UNIX_DYNAMIC_DS set to true in CMakeLists.txt for x11 to work)
        - libwayland-dev
        - CMake
2. Clone the repository
3. Navigate to project directory and build the project using CMake

## Contribution
[TODO]


## License
JACE is licensed under the MIT License, see [LICENSE.txt](https://github.com/Hedge239/JustAnotherCodeEditor/blob/latest-dev/LICENSE.txt) for more information
> Copyright (c) 2023-2024 Lucas I Kohorst
