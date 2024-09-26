#pragma once

#include <glm/glm.hpp>
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
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
};
