# Just Another Code Editor (JACE)
[![CodeFactor](https://www.codefactor.io/repository/github/hedge239/justanothercodeeditor/badge/latest-dev)](https://www.codefactor.io/repository/github/hedge239/justanothercodeeditor/overview/latest-dev)


JACE, short for Just Another Code Editor, is a open-source text editor design to write code while being cross-platform and light-weight and efficient on system resources.

## Features
- Lightweight: Made in C++, JACE uses less system resources compared to other editors
- Modular: Designed with modularity at its core, JACE offers extensive customization and a expandable plugin system
- Cross-platform: Designed to run on both Windows and Linux operating systems

## Instalation
Pre-commpiled versions can be downloaded for various operating systems are abailable for download here[LINK, TODO].Additionally "Offical" plugins can be found [here](https://github.com/Hedge239/JACEplugins)

## Building from Source
If you wish to build JACE from scratch, nsure you meet the prerequisites outlined below and then run the provided CMakeLists.txt

#### Prerequisites (Windows Only)
- Win32
    - Have `JACE_SUPPORT_WINAPI` set to `ON` in CMakeLists.txt

#### Prerequisites (Linux Only)
> Note:Both X11 and Wayland support can be enabled simultaneously, albeit increasing the application's size.
- X11
    - X11 Development Lib
    - Have the `JACE_SUPPORT_x11` set to `ON` in CMakeLists.txt
- Wayland
    - Wayland Development Lib
    - Have the `JACE_SUPPORT_WAYLAND` set to `ON` in CMakeLists.txt

## Contribution
We welcome contributions from anyone interested in supporting JACE, Please refer to [CONTRIBUTING.md](https://github.com/Hedge239/JustAnotherCodeEditor/blob/latest-dev/docs/CONTRIBUTING.md) for detailed info.


## License
JACE is licensed under the MIT License, see [LICENSE.txt](https://github.com/Hedge239/JustAnotherCodeEditor/blob/latest-dev/LICENSE.txt) for more information
> Copyright (c) 2023-2024 Lucas I Kohorst
