#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <wayland-client.h>

// #include <EGL/egl.h>
// #include <EGL/eglext.h>

// #include <glad/glad_egl.h>
#include <glad/glad.h>
#include "egl_common.h"
// #include <GL/glew.h>

EGLDisplay egl_display;
EGLConfig egl_config;
EGLContext egl_context;

// typedef EGLDisplay (EGLAPIENTRYP PFNEGLGETPLATFORMDISPLAYEXTPROC) (EGLenum platform, void *native_display, const EGLint *attrib_list);
PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC eglCreatePlatformWindowSurfaceEXT;

const EGLint config_attribs[] = {
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	EGL_RED_SIZE, 1,
	EGL_GREEN_SIZE, 1,
	EGL_BLUE_SIZE, 1,
    EGL_SAMPLES, 4,
	EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
	EGL_NONE,
};

const EGLint context_attribs[] = {
	EGL_CONTEXT_MAJOR_VERSION, 3,
    EGL_CONTEXT_MINOR_VERSION, 2,
    EGL_NONE
};

bool egl_init(struct wl_display *display) {
    const char* clientAPI = NULL;   
	const char *client_exts_str =
		eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

	EGLint matched = 0;
    // Initialize GLEW

	if (client_exts_str == NULL) {
		if (eglGetError() == EGL_BAD_DISPLAY) {
			fprintf(stderr, "EGL_EXT_client_extensions not supported\n");
		} else {
			fprintf(stderr, "Failed to query EGL client extensions\n");
		}
		return false;
	}

	if (!strstr(client_exts_str, "EGL_EXT_platform_base")) {
		fprintf(stderr, "EGL_EXT_platform_base not supported\n");
		return false;
	}

	if (!strstr(client_exts_str, "EGL_EXT_platform_wayland")) {
		fprintf(stderr, "EGL_EXT_platform_wayland not supported\n");
		return false;
	}

	eglGetPlatformDisplayEXT =
		(PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
	if (eglGetPlatformDisplayEXT == NULL) {
		fprintf(stderr, "Failed to get eglGetPlatformDisplayEXT\n");
		return false;
	}

	eglCreatePlatformWindowSurfaceEXT =
		(PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
	if (eglCreatePlatformWindowSurfaceEXT == NULL) {
		fprintf(stderr, "Failed to get eglCreatePlatformWindowSurfaceEXT\n");
		return false;
	}

	egl_display =
		eglGetPlatformDisplayEXT(EGL_PLATFORM_WAYLAND_EXT,
			display, NULL);
	if (egl_display == EGL_NO_DISPLAY) {
		fprintf(stderr, "Failed to create EGL display\n");
		goto error;
	}

	if (eglInitialize(egl_display, NULL, NULL) == EGL_FALSE) {
		fprintf(stderr, "Failed to initialize EGL\n");
		goto error;
	}


	if (!eglChooseConfig(egl_display, config_attribs,
			&egl_config, 1, &matched)) {
		fprintf(stderr, "eglChooseConfig failed\n");
		goto error;
	}
	if (matched == 0) {
		fprintf(stderr, "Failed to match an EGL config\n");
		goto error;
	}

	egl_context =
		eglCreateContext(egl_display, egl_config,
			EGL_NO_CONTEXT, context_attribs);
	if (egl_context == EGL_NO_CONTEXT) {
		fprintf(stderr, "Failed to create EGL context\n");
        switch(eglGetError()) {
            case EGL_SUCCESS:
                printf("Success\n");
                break;
            case EGL_NOT_INITIALIZED:
                printf("EGL_NOT_INITIALIZED: EGL display not initialized\n");
                break;
            case EGL_BAD_ACCESS:
                printf("EGL_BAD_ACCESS: EGL cannot access a requested resource\n");
                break;
            case EGL_BAD_ALLOC:
                printf("EGL_BAD_ALLOC: EGL failed to allocate resources for the requested operation\n");
                break;
            case EGL_BAD_ATTRIBUTE:
                printf("EGL_BAD_ATTRIBUTE: EGL attribute is not recognized or specified correctly\n");
                break;
            case EGL_BAD_CONTEXT:
                printf("EGL_BAD_CONTEXT: EGL context is not valid or not compatible with the current operation\n");
                break;
            case EGL_BAD_CONFIG:
                printf("EGL_BAD_CONFIG: EGL configuration is not valid\n");
                break;
            case EGL_BAD_CURRENT_SURFACE:
                printf("EGL_BAD_CURRENT_SURFACE: The current surface of the calling thread is no longer valid\n");
                break;
            case EGL_BAD_DISPLAY:
                printf("EGL_BAD_DISPLAY: EGL display connection is not valid\n");
                break;
            case EGL_BAD_SURFACE:
                printf("EGL_BAD_SURFACE: EGL surface is not valid\n");
                break;
            case EGL_BAD_MATCH:
                printf("EGL_BAD_MATCH: EGL argument mismatch or context/surface mismatch\n");
                break;
            case EGL_BAD_PARAMETER:
                printf("EGL_BAD_PARAMETER: EGL parameter is not valid or outside the valid range\n");
                break;
            case EGL_BAD_NATIVE_PIXMAP:
                printf("EGL_BAD_NATIVE_PIXMAP: EGL native pixmap is not valid\n");
                break;
            case EGL_BAD_NATIVE_WINDOW:
                printf("EGL_BAD_NATIVE_WINDOW: EGL native window is not valid\n");
                break;
            case EGL_CONTEXT_LOST:
                printf("EGL_CONTEXT_LOST: EGL context has been lost\n");
                break;
                };
		goto error;
	}

    if (!eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_context)) {
        fprintf(stderr, "Failed to make EGL context current\n");
        goto error;
    }

    gladLoadGLLoader((GLADloadproc)eglGetProcAddress);

	return true;


error:
	eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE,
		EGL_NO_SURFACE, EGL_NO_CONTEXT);
	if (egl_display) {
		eglTerminate(egl_display);
	}
	eglReleaseThread();
	return false;
}

void egl_finish(void) {
	eglMakeCurrent(egl_display, EGL_NO_SURFACE,
		EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(egl_display, egl_context);
	eglTerminate(egl_display);
	eglReleaseThread();
}
