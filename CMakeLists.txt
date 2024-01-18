# Only needs to hold until I finish BuildToolsForC+, then this goes to life support
# Also taking ALOT of code from ZephyrUI, as this only took priority because I have a growing annoyance with VS-Code.
# Allthough the goal atleast for now, is to have ZephyrUI in a fuctional state and moving the UI system to that by version 2.0.0, witch should be easy since its based off it allready.

cmake_minimum_required(VERSION 3.5.0)
project(JACE VERSION 0.0.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/.buildtools/.cmake/modules")

# Toggle off to not include X11
option(LINUX_DYNAMIC_DS "Build the project to use both X11 and Wayland" ON)

include(".buildtools/cmake/app.cmake")

if(WIN32)
    message(STATUS "Display Desktop: Win32")
    message(STATUS "Display Server: Win32")
    include(".buildtools/cmake/platform/windows.cmake")
elseif(UNIX)
    message(STATUS "Display Desktop: LINUX")
    include(".buildtools/cmake/platform/linux.cmake")

    if(LINUX_DYNAMIC_DS)
        message(STATUS "Display Server Mode: DYNAMIC")
        message(STATUS "Display Server: WAYLAND/X11 HYBRID")

        include(".buildtools/cmake/platforms/x11.cmake")
        include(".buildtools/cmake/platforms/wayland.cmake")

        find_package(X11 REQUIRED)
        find_package(Wayland REQUIRED)

        list(APPEND DEPENDENCES_INCLUDE "${X11_X11_INCLUDE_PATH}")
        list(APPEND DEPENDENCES_INCLUDE "${WAYLAND_INCLUDE_DIRS}")

        list(APPEND DEPENDENCES_LIB "${X11_X11_LIB}")
        list(APPEND DEPENDENCES_SRC "${WAYLAND_LIBRARIES}")

        list(APPEND BUILD_FLAGS "wayland-client")
        list(APPEND BUILD_FLAGS "X11")

        add_compile_definitions(UNIX_DYNAMIC_DS)
    else()
        message(STATUS "Display Server Mode: MODERN")
        message(STATUS "Display Server: WAYLAND")

        find_package(Wayland REQUIRED)

        list(APPEND DEPENDENCES_INCLUDE "${WAYLAND_INCLUDE_DIRS}")
        list(APPEND DEPENDENCES_SRC "${WAYLAND_LIBRARIES}")

        list(APPEND BUILD_FLAGS "wayland-client")
    endif()
else()
    message(FATAL_ERROR "Faild to find valid display service")
endif()

add_executable(${PROJECT_NAME} "src/app.cpp" ${PLATFORM_SRC} ${APP_SRC})

target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SRC} ${APP_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../JACE_app")

target_include_directories(${PROJECT_NAME} PRIVATE "include" ${DEPENDENCES_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDENCES_SRC} ${BUILD_FLAGS})

# Copy files in Assets
set(APP_ASSETS
"assets/gfx"
"assets/locales"
)

file(COPY ${APP_ASSETS} DESTINATION "${CMAKE_BINARY_DIR}/../JACE_app")