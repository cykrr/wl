#include <stdio.h>
#include <chrono>
#include <ctime>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib/graph2/include/program.hpp"

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"
#include "nanovg_gl.h"


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

int main() {
    if (!glfwInit()) return 1;
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_WAYLAND_SHELL_LAYER, ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY);
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

    NVGcontext *vg = nvglCreate(NVG_SRGB | NVG_AUTOW_DEFAULT);
    
    // Convert the time_t object to a local time struct

    auto f = [](int x){
        float norm = (float)x/12;
        float angle = norm*2*M_PI + 3*M_PI_2;
        return angle;
    };

    auto g = [](int x){
        float norm = (float)x/60;
        float angle = norm*2*M_PI + 3*M_PI_2;
        return angle;
    };
    

    int cx = 300, cy = 1400;

    while (!glfwWindowShouldClose(w)) {

        auto now = std::chrono::system_clock::now();
        // Convert the system time to a time_t object
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        struct std::tm* localTime = std::localtime(&currentTime);
        
        // Extract the hours and minutes from the local time struct
        int hours = localTime->tm_hour;
        int minutes = localTime->tm_min;


        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0,0.0,0.0,0.0);
        nvgBeginFrame(vg, width, height, 1.0);


        nvgFontSize(vg,200.0f);
        nvgFontFace(vg,"sans-bold");
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgFillColor(vg,nvgRGBA(254,255,255,255));
        nvgText(vg, cx + 400,cy ,"Hola",NULL);

        nvgStrokeWidth(vg, 5.0f);
        nvgStrokeColor(vg, nvgRGBA(254,255,255,254));

        nvgBeginPath(vg);
        nvgCircle(vg, cx, cy, 200);
        nvgStroke(vg);

        nvgFillColor(vg, nvgRGBA(0,0,0, 100));
        nvgBeginPath(vg);
        nvgCircle(vg, cx, cy, 200);
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgMoveTo(vg, cx + cos(f(hours))*0.5*200,
                cy + sin(f(hours))*0.5*200);
        nvgLineTo(vg, cx,cy);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgMoveTo(vg, cx + cos(g(minutes))*0.8*200,
                cy + sin(g(minutes))*0.8*200);
        nvgLineTo(vg, cx,cy);
        nvgStroke(vg);




        nvgEndFrame(vg);

        glfwPollEvents();
        glfwSwapBuffers(w);
    }
    
    
}

