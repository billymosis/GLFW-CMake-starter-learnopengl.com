// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "./shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window, float deltaTime);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

unsigned int loadTexture(char const *path);

int window_height = 800;
int window_width = 600;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX = window_height / 2.0f;
double lastY = window_width / 2.0f;
bool firstMouse = false;
bool enableMouse = false;

// lighting
glm::vec3 lightPos(0.0f, 0.25f, 0.0f);

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
  glfwSetErrorCallback(glfw_error_callback);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // GLFW window creation
  GLFWwindow *window =
      glfwCreateWindow(window_height, window_width, "Learn OPENGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwGetFramebufferSize(window, &window_width, &window_height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  // GLAD load all OpenGL function pointer
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // tell stb_image.h to flip loaded texture's on the y-axis (before loading
  // model).
  stbi_set_flip_vertically_on_load(true);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile shaders
  // -------------------------
  Shader ourShader("../shaders/model_loading_vs.glsl",
                   "../shaders/model_loading_fs.glsl");

  // load models
  // -----------
  const char *modelPath = "../assets/backpack/backpack.obj";
  Model ourModel(modelPath);

  while (!glfwWindowShouldClose(window)) {
    // imgui logic
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window, deltaTime);

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), (float)window_width / (float)window_height,
        0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(
        model,
        glm::vec3(
            0.0f, 0.0f,
            0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(
        model,
        glm::vec3(1.0f, 1.0f,
                  1.0f)); // it's a bit too big for our scene, so scale it down
    ourShader.setMat4("model", model);
    ourModel.Draw(ourShader);

    {
      ImGui::Begin("SETTINGS");
      ImGui::Checkbox("Enable Mouse", &enableMouse);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  // imgui cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  window_width = width;
  window_height = height;
}

void processInput(GLFWwindow *window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  const float cameraSpeed = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
};

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = xposIn;
  float ypos = yposIn;
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xoffset = xpos - lastX;
  // reversed since y-coordinates go from bottom to top
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;
  if (enableMouse) {
    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
};

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    stbi_image_free(data);
  }

  return textureID;
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    enableMouse = !enableMouse;
  }
}
