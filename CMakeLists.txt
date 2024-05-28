cmake_minimum_required(VERSION 3.5.0)
project(JACE VERSION 0.0.1)

# Windows
option(SUPPORT_WINAPI "Includes Win32 Support" ON)
# Linux
option(SUPPORT_WAYLAND "Includes Wayland Support" ON)
option(SUPPORT_X11 "Includes x11 Support" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/build_tools/cmake/modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include("build_tools/cmake/app.cmake")


# Display Service
if(WIN32)
    message(STATUS "Operating System: Windows")
    list(APPEND APP_SRC ${WINDOWS_SRC})

    if(SUPPORT_WINAPI)
        message(STATUS "Enabled Display Service: Win32")
        set(hasDisplayService true)

        include("build_tools/cmake/platforms/Win32.cmake")
        list(APPEND DEPENDENCES_SRC Comctl32)

        add_compile_definitions(SUPPORT_WINAPI)
    endif()
endif()

if(LINUX)
    message(STATUS "Operating System: Linux")
    list(APPEND APP_SRC ${LINUX_SRC})

    if(SUPPORT_X11)
        message(STATUS "Enabled Display Service: X11")
        set(hasDisplayService true)

        find_package(X11 REQUIRED)

        include("build_tools/cmake/platforms/x11.cmake")
        list(APPEND DEPENDENCES_INCLUDE "${X11_X11_INCLUDE_PATH}")
        list(APPEND DEPENDENCES_LIB "${X11_X11_LIB}")
        list(APPEND BUILD_FLAGS "X11")

        add_compile_definitions(SUPPORT_X11)
    endif()

    if(SUPPORT_WAYLAND)
        message(STATUS "Enabled Display Service: Wayland")
        set(hasDisplayService true)

        find_package(Wayland REQUIRED)

        include("build_tools/cmake/platforms/wayland.cmake")
        list(APPEND DEPENDENCES_INCLUDE "${WAYLAND_INCLUDE_DIRS}")
        list(APPEND DEPENDENCES_SRC "${WAYLAND_LIBRARIES}")
        list(APPEND BUILD_FLAGS "wayland-client")

        add_compile_definitions(SUPPORT_WAYLAND)
    endif()
endif()

if(NOT hasDisplayService)
    message(FATAL_ERROR "No Display Service Enabled")
endif()


# Executable
add_executable(${PROJECT_NAME} "src/app.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SRC} ${APP_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "JACE_app")

target_include_directories(${PROJECT_NAME} PRIVATE "include" ${DEPENDENCES_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDENCES_SRC} ${BUILD_FLAGS})

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()


# Transfer Assets
file(COPY "assets/" DESTINATION "${CMAKE_BINARY_DIR}/JACE_app")