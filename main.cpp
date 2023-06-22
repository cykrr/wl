#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib/graph2/include/program.hpp"

enum zwlr_layer_shell_v1_layer {
	ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND = 0,
	ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM = 1,
	ZWLR_LAYER_SHELL_V1_LAYER_TOP = 2,
	ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY = 3,
};

const float vertices[] = {
    -1.0, -1.0, 0.0,  // Bottom left corner
    -1.0, 1.0, 0.0,   // Top left corner
    1.0, -1.0, 0.0,   // Bottom right corner
    1.0, 1.0, 0.0     // Top right corner
};

// const unsigned int indices[] = {
//     0, 1, 2,   // First triangle (bottom left, top left, bottom right)
//     1, 3, 2    // Second triangle (top left, top right, bottom right)
// };

static unsigned int width = 1, height = 1;

static Program *program = NULL;


int main() {
    if (!glfwInit()) return 1;
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    // glfwWindowHint(GLFW_WAYLAND_SHELL_LAYER, ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    GLFWwindow *w = glfwCreateWindow(1 , 1,"krr",NULL,NULL);
    glfwMakeContextCurrent(w);

    glfwSetFramebufferSizeCallback(w, [] (GLFWwindow *wnd, int w, int h) {
        glViewport(0, 0, w, h);
        width=w;
        height=h;
    });

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        printf("Error loading GLAD\n");
    else
        printf("%s\n", glGetString(GL_VERSION));

    program = new Program("main");
    program->use();
    program->setVec2("center", glm::vec2(width/2,height/2));
    program->setFloat("radius", 0.5);
    unsigned int vao, vbo, ebo;
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(w)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0,0.0,0.0,0.0);
        program->use();
        program->setVec2("iRes", width, height);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glfwPollEvents();
        glfwSwapBuffers(w);
    }
    
    
}

