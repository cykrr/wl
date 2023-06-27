#include <stdio.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib/graph2/include/program.hpp"
#include "lib/yoga/yoga/YGNode.h"

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"
#include "nanovg_gl.h"

#include "yoga/Yoga.h"
#include "demo.h"

#include "view.h"
#include "clock.h"

#include "lib/glfw/src/internal.h"
#include "lib/glfw/src/wl_platform.h"


enum zwlr_layer_shell_v1_layer {
	ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND = 0,
	ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM = 1,
	ZWLR_LAYER_SHELL_V1_LAYER_TOP = 2,
	ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY = 3,
};

static unsigned int width = 1280, height = 720;
static unsigned int mx = 0, my = 0;

int main() {
    View root = View();
        root.set_width(width);
        root.set_height(height);
        root.set_x(0);
        root.set_y(0);
        // root.set_padding(YGEdgeAll, 0);
    View clock_holder = View();
        clock_holder.set_flex_direction(YGFlexDirectionColumnReverse);
        clock_holder.set_padding(YGEdgeAll, 20);
    root.add_child(clock_holder);

    if (!glfwInit()) return 1;
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_WAYLAND_SHELL_LAYER, ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow *w = glfwCreateWindow(2 , 2,"krr",NULL,NULL);
    glfwMakeContextCurrent(w);
    glfwSetWindowUserPointer(w, &root);

    float scale;
    glfwGetWindowContentScale(w, &scale, &scale);
    printf("Userscale%.2f\n", scale);
    scale=2;

    glfwSetFramebufferSizeCallback(w, [] (GLFWwindow *wnd, int w, int h) {
        View* root = (View*)glfwGetWindowUserPointer(wnd);
        width=(float)w;
        height=(float)h;
        root->set_width(width);
        root->set_height(height);
        root->update_node();
        glViewport(0, 0, width, height);
        printf("fb resize %d %d\n",width,height);
    });


    glfwSetWindowSizeCallback(w, [] (GLFWwindow *wnd, int w, int h) {
        // glViewport(0, 0, w, h);
        // width=w;
        // height=h;
        printf("Window resize %d %d\n",w,h);
    });

    glfwSetCursorPosCallback(w, [] (GLFWwindow *wnd, double x, double y) {
        mx=x;my=y;
        
    });

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        printf("Error loading GLAD\n");
    else
        printf("%s\n", glGetString(GL_VERSION));

    NVGcontext *vg = nvglCreate(NVG_SRGB | NVG_AUTOW_DEFAULT);

    int fontHandle = nvgCreateFont(vg, "DroidSans", "/usr/share/fonts/droid/DroidSans.ttf");
    if (fontHandle == -1) {
        printf("error");
        // Handle font loading failure
        // Add error handling code here
    }
    



    Clock clk(vg, scale);
        clock_holder.add_child(clk);


    DemoData data;
    loadDemoData(vg, &data);

    while (!glfwWindowShouldClose(w)) {
        double x,y;
        glfwGetCursorPos(w,&x,&y);
        // printf("Mouse %.0f, %.0f\n",x, y);
        root.update_node();


        clk.update_time();

        // nvgBeginPath(vg);
        // nvgRect(vg, clk.get_x(), clk.get_y(), clk.get_width(), clk.get_height());
        // nvgStroke(vg);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0,0.0,0.0,1.0);
        nvgBeginFrame(vg, width, height, 1.);
        clk.draw();
        // renderDemo(vg, mx, my, width, height, glfwGetTime(), false, &data);
        nvgEndFrame(vg);

        //
        // nvgFillColor(vg, nvgRGBA(254,255,255,254));
        //
        nvgBeginPath(vg);
        nvgRect(vg, 0, 0, 1920, 1080);
        nvgStroke(vg);

        nvgFontSize(vg,15.0f * 2.);
        nvgFontFace(vg,"1920x1080");
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgFillColor(vg,nvgRGBA(254,255,255,255));
        nvgText(vg, 0 + 1920/2, 0 + 1080/2, "1920x1080", NULL); 
        //
        //


        glfwPollEvents();
        glfwSwapBuffers(w);
    }
    
    
}

