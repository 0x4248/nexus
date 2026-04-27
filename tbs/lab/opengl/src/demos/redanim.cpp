#include <GLFW/glfw3.h>
#include <iostream>

int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Hello OpenGL", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  int x = 0;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(x / 255.0f, 0.0f, 0.0f, 1.0f); // Animated red screen
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
    x = (x + 1) % 256;
  }

  glfwTerminate();
  return 0;
}
