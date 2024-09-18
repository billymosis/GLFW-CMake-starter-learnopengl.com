// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "./shader.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
  std::cout << "Hello World!" << std::endl;
  // GLFW initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // GLFW window creation
  GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OPENGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD load all OpenGL function pointer
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Shader ourShader = Shader("../shaders/vertex_shader.glsl",
                            "../shaders/fragment_shader_orange.glsl");

  // clang-format off
  float vertices1[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };
  unsigned int indices[] = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };
  // clang-format on

  GLuint VBOs[2], VAOs[2], EBO;
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);
  glGenBuffers(1, &EBO);

  // FIRST TRIANGLE
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAOs[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  // sizeof vertices triangle 3x3 = 9 x 4 byte = 36 byte
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  /**
   * We need to tell opengl how to intepret the vertex data before rendering
   * Look at the vertex buffer data image on the pdf or site learnopengl.com
   *
   *
   * EXPLANATION
   * 1. First Args 0
   * This is specify which vertex attribute we want to configure.
   * We already set on the vertex shader source that the layout location = 0.
   * This sets the location of the vertex attribute to 0 and since we want to
   * pass data to this vertex attribute, we pass in 0.
   *
   * 2. Second Args 3
   * Specify the size of the vertex attribute.
   * The vertex attribute is a vec3 so it is composed of 3 values.
   *
   * 3. The type of the vertex data, commonly float.
   *
   * 4. Specify if the data to be normalized or not.
   * if we input integer, the integer will be normalized to 0 or 1 when
   converted to float.
   * We don't want this since irrelevant.
   *
   * 5. Stride, see the image.
   * This tells OpenGL the spacing between consecutive vertex attributes in the
   * buffer. Since each vertex consists of 3 floats (one for x, one for y, and
   * one for z), the stride is 3 * sizeof(float) bytes (each float is typically
   * 4 bytes, so stride is 12 bytes).
   *
   * 6. type of void* need a weird cast.
   * This is the offset of where the position data begins in buffer.
   * Since the position data is at the start of the data array this values is
   just 0.
   *
   **/
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

  //  Zero => Refers to the same attribute index 0 specified in
  //  glVertexAttribPointer(). It enables the vertex attribute at this location
  //  so that the data will be passed to the vertex shader.
  glEnableVertexAttribArray(0);

  // set the new color attribute
  // last argument tells that we need offset by 3x4 bytes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // set texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  // Generating a texture
  GLuint texture[2];
  glGenTextures(1, &texture[0]);
  glGenTextures(2, &texture[0]);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);

  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load Texture

  stbi_set_flip_vertically_on_load(true);
  // nrChannels => Number of Color Channels
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("../assets/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  glBindTexture(GL_TEXTURE_2D, texture[1]);
  data =
      stbi_load("../assets/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);
  // tell opengl for each sampler to which texture unit it belongs to (only has
  // to be done once)
  // -------------------------------------------------------------------------------------------
  ourShader.use(); // don't forget to activate/use the shader before setting
                   // uniforms!
  // either set it manually like so:
  glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
  // or set it via the texture class
  ourShader.setInt("texture2", 1);

  while (!glfwWindowShouldClose(window)) {
    // Input
    processInput(window);

    // Rendering commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind texture
    // glBindTexture(GL_TEXTURE_2D, texture[0]);
    // glBindTexture(GL_TEXTURE_2D, texture[1]);

    // Draw triangle
    ourShader.use();

    // Draw 1st triangle
    // seeing as we only have a single VAO there's no need to bind it every
    // time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   0); // set the count to 6 since we're drawing
                       // 6 vertices now (2 triangles); not 3!

    // check and call envents and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
