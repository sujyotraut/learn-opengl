#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "stb_image.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *vertexShaderPath = "../../shaders/vertex_shader.glsl";
const char *fragmentShaderPath = "../../shaders/fragment_shader.glsl";
const char *imagePath0 = "../../images/container.png";
const char *imagePath1 = "../../images/awesomeface.png";

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned char *loadImage(const char *file_name, int *width, int *height);

int main() {
#pragma region Setup GLFW
  // Initialize glfw
  if (!glfwInit()) {
    std::cout << "Failed to initialize glfw" << std::endl;
    return 0;
  }

  // Use OpenGL 4.5
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

  /* OpenGL provides different profiles that define a set of features and capabilities.
   * The most common profiles are the core profile and the compatibility profile.
   * Core profile: The core profile defines the essential features of OpenGL.
   * It provides a minimal set of functionality and is recommended for modern applications.
   * Compatibility profile: The compatibility profile includes all the features of the core profile, as well as
   * additional features that are deprecated or removed in the core profile.
   * It is recommended for applications that need to support older hardware or legacy features. */
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }

  /* When you create a GLFW window, it also creates an OpenGL context associated with that window.
   * However, this context is not automatically made current, meaning that OpenGL commands will not be directed to this
   * context by default. Calling `glfwMakeContextCurrent(window)` means that any subsequent OpenGL commands will be
   * executed on this context, and will be rendered to the specified window. If you have multiple windows, each with its
   * own OpenGL context, you need to make the correct context current before issuing OpenGL commands. This ensures that
   * the commands are executed on the correct window. */
  glfwMakeContextCurrent(window);

  /* `glfwGetProcAddress("glClear")` will return an address to an OpenGL `glClear()` function. */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Set glfw callbacks
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
#pragma endregion

#pragma region Setup VAO, VBO, EBO
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  // VAO is required in OpenGL core profile.
  // For OpenGL compatibility profile there is default VAO.
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Store attribute info and the bounded VBO (id/name) in a VAO.
  // We can unbound VBO, since it's info is stored in VAO.
  unsigned int stride = 5 * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind buffers
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#pragma endregion

#pragma region Setup Texture
  stbi_set_flip_vertically_on_load(true);

  unsigned int texture0, texture1;
  glGenTextures(1, &texture0);
  glGenTextures(1, &texture1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  int width0, height0;
  unsigned char *texture0Data = loadImage(imagePath0, &width0, &height0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, texture0Data);
  glGenerateTextureMipmap(texture0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width1, height1;
  unsigned char *texture1Data = loadImage(imagePath1, &width1, &height1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1Data);
  glGenerateTextureMipmap(texture1);

  stbi_image_free(texture0Data);
  stbi_image_free(texture1Data);
#pragma endregion

  // Model Matrix: Scale -> Rotate -> Translate
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

  // View Matrix
  glm::mat4 view = glm::mat4(1.0f);
  // note that we're translating the scene in the reverse direction of where we want to move
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  // Projection Matrix
  float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

#pragma region Setup Shader
  Shader shader(vertexShaderPath, fragmentShaderPath);

  shader.use();
  shader.setInt("texture0", 0);
  shader.setInt("texture1", 1);
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
#pragma endregion

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    /* When an event occurs, GLFW stores it in an internal event queue.
     * `glfwPollEvents()` is used to process the events in the queue.
     * Here's what `glfwPollEvents()` does:
     * 1. Check the event queue: GLFW checks the event queue for pending events.
     * 2. Process each event: For each event in the queue, GLFW calls the corresponding callback function.
     * 3. Clear the event queue: After processing all events, GLFW clears the event queue. */
    glfwPollEvents();
    process_input(window);

    glClearColor(.196f, .196f, .196f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glBindVertexArray(VAO);

    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    // It will try to draw triangles by grouping the vertices in sets of 3, any extra vertices will be ignored.
    // For example if the vertex buffer contains 4 vertices, last one is ignored
    unsigned int numberOfVertices = sizeof(vertices) / (5 * sizeof(float));
    glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

    // unsigned int numberOfIndices = sizeof(indices) / sizeof(unsigned int);
    // glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

    /* In OpenGL, a window typically has two buffers: the front buffer and the back buffer.
     * The front buffer is the buffer that is currently being displayed on the screen.
     * The back buffer is the buffer that is being drawn to.
     * When we call `glfwSwapBuffers(window)` the back buffer becomes the front buffer,
     * and the front buffer becomes the back buffer. */
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

unsigned char *loadImage(const char *file_name, int *width, int *height) {
  int nrChannels;
  unsigned char *textureData = stbi_load(file_name, width, height, &nrChannels, 0);

  if (!textureData) {
    std::cout << "Failed to load texture" << std::endl;
    glfwTerminate();
    return 0;
  }

  return textureData;
}
