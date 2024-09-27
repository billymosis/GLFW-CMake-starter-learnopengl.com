cmake_minimum_required(VERSION 3.5)
project(gl_learn)

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

# Include imgui 
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add GLAD
include_directories("${PROJECT_SOURCE_DIR}/external/glad/include")
set(GLAD_SOURCES "${PROJECT_SOURCE_DIR}/external/glad/src/glad.c")

# Include assimp
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Disable Assimp warnings as errors")
add_subdirectory(external/assimp)

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
set(lear_gl_src
    src/main.cpp
    src/shader.cpp
    src/shader.hpp
    src/stb_image.h
    src/model.hpp
    src/model.cpp
    src/mesh.cpp
    src/mesh.hpp
    ${IMGUI_SOURCES}
)

# Create the executable
# add_executable(GLFW-CMake-starter WIN32 ${GLFW-CMAKE-STARTER-SRC} ${GLAD_SOURCES})
add_executable(learn_gl ${lear_gl_src} ${GLAD_SOURCES})

# Link libraries
target_link_libraries(learn_gl ${OPENGL_LIBRARIES} glfw glm assimp)

# Set Visual Studio startup project
if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter)
    endif()
endif()
