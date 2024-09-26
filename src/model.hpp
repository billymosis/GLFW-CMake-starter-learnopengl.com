#pragma once

#include "mesh.hpp"
#include "shader.hpp"
#include "stb_image.h"
#include <assimp/scene.h>
#include <string>
#include <vector>

class Model {
public:
  Model(const char *path) { loadModel(path); }
  void Draw(Shader &shader);
  unsigned int TextureFromFile(const char *path, const std::string &directory,
                               bool gamma = false);

private:
  // model data
  std::vector<Texture> textures_loaded; 
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);
};

