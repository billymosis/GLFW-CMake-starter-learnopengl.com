cmake_minimum_required(VERSION 3.5)
project(GLFW-CMake-starter)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

# Include GLFW
add_subdirectory(external/glfw)

# Set GLM options
set(GLM_TEST_ENABLE OFF CACHE BOOL "GLM no test")

# Include GLM
add_subdirectory(external/glm)

# Add GLAD
include_directories("${PROJECT_SOURCE_DIR}/external/glad/include")
set(GLAD_SOURCES "${PROJECT_SOURCE_DIR}/external/glad/src/glad.c")

# Uncomment this to use default glad dependencies inside glfw
# option( GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" ON )
#
# if( GLFW-CMAKE-STARTER-USE-GLFW-GLAD )
#     include_directories("${GLFW_SOURCE_DIR}/deps")
#     set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
# endif()

# Linker flags for MSVC
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

# Source files for your project
set(GLFW-CMAKE-STARTER-SRC
    src/main.cpp
    src/shader.cpp
    src/shader.hpp
    src/stb_image.h
)

# Create the executable
# add_executable(GLFW-CMake-starter WIN32 ${GLFW-CMAKE-STARTER-SRC} ${GLAD_SOURCES})
add_executable(GLFW-CMake-starter ${GLFW-CMAKE-STARTER-SRC} ${GLAD_SOURCES})

# Link libraries
target_link_libraries(GLFW-CMake-starter ${OPENGL_LIBRARIES} glfw glm)

# Set Visual Studio startup project
if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter)
    endif()
endif()
