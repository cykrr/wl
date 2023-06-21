#ifndef _EGL_COMMON_H
#define _EGL_COMMON_H

#include <stdbool.h>
#include <wayland-client.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
// #include "lib/graph2/include/glad/glad_egl.h"

typedef EGLSurface (EGLAPIENTRYP PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC) (EGLDisplay dpy, EGLConfig config, void *native_window, const EGLint *attrib_list);
typedef EGLSurface (EGLAPIENTRYP PFNEGLCREATEPLATFORMPIXMAPSURFACEEXTPROC) (EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLint *attrib_list);

extern EGLDisplay egl_display;
extern EGLConfig egl_config;
extern EGLContext egl_context;

extern PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC eglCreatePlatformWindowSurfaceEXT;

bool egl_init(struct wl_display *display);

void egl_finish(void);

#endif
