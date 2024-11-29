#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main() {
  // Initialize glfw
  if (!glfwInit()) {
    std::cout << "Failed to initialize glfw" << std::endl;
    return 0;
  }

  // Use OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  /* OpenGL provides different profiles that define a set of features and capabilities.
   * The most common profiles are the core profile and the compatibility profile.
   * Core profile: The core profile defines the essential features of OpenGL.
   * It provides a minimal set of functionality and is recommended for modern applications.
   * Compatibility profile: The compatibility profile includes all the features of the core profile, as well as
   * additional features that are deprecated or removed in the core profile.
   * It is recommended for applications that need to support older hardware or legacy features. */
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

  // Set window resize callback
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
    // First triangle
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, 0.5f, 0.0f, // top right
    -0.5f, 0.5f, 0.0f, // top left
    // Second triangle
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
  };

  // Create & bind VAO (Vertex Array Object)
  // It is required in OpenGL core profile.
  // For OpenGL compatibility profile there is default VAO.
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Create & bind VBO (Vertex Buffer Object)
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Store vertex data in currently bonded buffer (VBO)
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Store attribute info and the bounded VBO (id/name) in a VAO.
  // We can unbound VBO, since it's info is stored in VAO.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

#pragma region Shader
  const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
#pragma endregion

  while (!glfwWindowShouldClose(window)) {
    /* When an event occurs, GLFW stores it in an internal event queue.
     * `glfwPollEvents()` is used to process the events in the queue.
     * Here's what `glfwPollEvents()` does:
     * 1. Check the event queue: GLFW checks the event queue for pending events.
     * 2. Process each event: For each event in the queue, GLFW calls the corresponding callback function.
     * 3. Clear the event queue: After processing all events, GLFW clears the event queue. */
    glfwPollEvents();
    process_input(window);

    glClearColor(.196f, .196f, .196f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // It will try to draw triangles by grouping the vertices in sets of 3, any extra vertices will be ignored.
    // For example if the vertex buffer contains 4 vertices, last one is ignored
    unsigned int numberOfVertices = sizeof(vertices) / (3 * sizeof(float));
    glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

    /* In OpenGL, a window typically has two buffers: the front buffer and the back buffer.
     * The front buffer is the buffer that is currently being displayed on the screen.
     * The back buffer is the buffer that is being drawn to.
     * When we call `glfwSwapBuffers(window)` the back buffer becomes the front buffer,
     * and the front buffer becomes the back buffer. */
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}