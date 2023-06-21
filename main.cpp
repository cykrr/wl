#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib/graph2/include/program.hpp"
#include "wayland-wlr-layer-shell-unstable-v1-client-protocol.h"
static float vertices[] = {
    -0.5, -0.5, 0.0,
    0.0, 0.5, 0.0,
    0.5, -0.5, 0.0,
};

int main() {
    if (!glfwInit()) return 1;
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_WAYLAND_SHELL_LAYER, ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    GLFWwindow *w = glfwCreateWindow(1 , 1,"krr",NULL,NULL);
    glfwMakeContextCurrent(w);

    glfwSetFramebufferSizeCallback(w, [] (GLFWwindow *wnd, int w, int h) {
        glViewport(0, 0, w, h);
    });

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        printf("Error loading GLAD\n");
    else
        printf("%s\n", glGetString(GL_VERSION));

    Program p("main");
    unsigned int vao, vbo;
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(w)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0,0.0,0.0,0.0);
        p.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(w);
    }
    
    
}

