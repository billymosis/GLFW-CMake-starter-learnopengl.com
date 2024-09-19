#pragma once

#include "glm/detail/type_mat.hpp"
#include <glad/glad.h>
#include <string>

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexPath, const char *fragmentPath);
  // Use or activate shaders
  void use();

  // Utility uniform funcions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, glm::mat4 &value) const;
};
