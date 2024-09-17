// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Function to read the shader from a file
std::string readShaderFromFile(const std::string &shaderPath) {
  std::ifstream shaderFile(shaderPath);
  if (!shaderFile.is_open()) {
    std::cerr << "Failed to open shader file: " << shaderPath << std::endl;
    return "";
  }

  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  shaderFile.close();
  return shaderStream.str();
}

// Function to compile shader
GLuint compileShader(GLenum shaderType, const std::string &shaderSource) {
  GLuint shader = glCreateShader(shaderType);
  const char *sourceCStr = shaderSource.c_str();
  glShaderSource(shader, 1, &sourceCStr, NULL);
  glCompileShader(shader);

  // Check for compilation errors
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "Error compiling shader:\n" << infoLog << std::endl;
  }

  return shader;
}
