# Only needs to hold until I finish BuildToolsForC+, then this goes to life support (Thank goodness)
# Also taking ALOT of code from ZephyrUI, as this only took priority because I have a growing annoyance with VS-Code.
# Allthough the goal atleast for now, is to have ZephyrUI in a fuctional state and moving the UI system to that by version 2.0.0, witch should be easy since its based off it allready.

cmake_minimum_required(VERSION 3.5.0)
project(JACE VERSION 0.0.1)

option(JACE_SUPPORT_WINAPI "Includes Win32 Support" ON)
option(JACE_SUPPORT_WAYLAND "Includes Wayland Support" ON)
option(JACE_SUPPORT_X11 "Includes X11 Support" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/buildtools/.cmake/modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include("buildtools/cmake/app.cmake")


# Display Server
if(WIN32)
    message(STATUS "Operating System: Windows")
    
    if(JACE_SUPPORT_WINAPI)
        message(STATUS "Supported Display Server: Win32")

        set(hasWindowingSystem true)

        include("buildtools/cmake/platform/windows.cmake")

        list(APPEND DEPENDENCES_SRC Comctl32)
        add_compile_definitions(SUPPORT_WINAPI)
    endif()

endif()

if(UNIX)
    message(STATUS "Operating System: UNIX")

    if(JACE_SUPPORT_X11)
        message(STATUS "Supported Display Server: X11")
        
        set(hasWindowingSystem true)
        
        find_package(X11 REQUIRED)
        include("buildtools/cmake/platforms/x11.cmake")

        list(APPEND DEPENDENCES_INCLUDE "${X11_X11_INCLUDE_PATH}")
        list(APPEND DEPENDENCES_LIB "${X11_X11_LIB}")

        list(APPEND BUILD_FLAGS "X11")
        add_compile_definitions(SUPPORT_X11)
    endif()

    if(JACE_SUPPORT_WAYLAND)
        message(STATUS "Supported Display Server: Wayland")

        set(hasWindowingSystem true)

        find_package(Wayland REQUIRED)
        include("buildtools/cmake/platforms/wayland.cmake")

        list(APPEND DEPENDENCES_INCLUDE "${WAYLAND_INCLUDE_DIRS}")
        list(APPEND DEPENDENCES_SRC "${WAYLAND_LIBRARIES}")

        list(APPEND BUILD_FLAGS "wayland-client")
        add_compile_definitions(SUPPORT_WAYLAND)
    endif()

endif()

if(NOT hasWindowingSystem)
    message(FATAL_ERROR "No Display Server Enabled")
endif()


# Create Executable
add_executable(${PROJECT_NAME} "src/app.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SRC} ${APP_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "JACE_app")

target_include_directories(${PROJECT_NAME} PRIVATE "include" ${DEPENDENCES_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDENCES_SRC} ${BUILD_FLAGS})

# After Show
file(COPY "assets/" DESTINATION "${CMAKE_BINARY_DIR}/JACE_app")

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()