#include <iostream>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <string.h>
#include <wayland-client-core.h>

static wl_display *wl_dpy;

void printEGLConfigAttributes(EGLDisplay dpy, EGLConfig conf);
static int load_egl();
static const char* getEGLErrorString(EGLint error);
static EGLDisplay egl_dpy;
static PFNEGLGETPLATFORMDISPLAYEXTPROC 
    eglGetPlatformDisplayEXT;
static PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC 
    eglCreatePlatformWindowSurfaceEXT;

static EGLint CONFIG_ATTRIBS[] = {
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_NONE
};



int main() {
    wl_dpy = wl_display_connect(NULL);

    if (!wl_dpy) {
        printf("Error creating dpy\n");
    } else {
        printf("Connected wl_dpy\n");
    }

    load_egl();

    EGLConfig config;
    EGLint config_length;
    if (eglChooseConfig(egl_dpy,CONFIG_ATTRIBS, &config,
                1, &config_length) == EGL_FALSE) {
        printf("Error choosing EGL config\n");
        printf("%s\n", getEGLErrorString(eglGetError()));
    }

    EGLSurface surface = eglCreatePbufferSurface

    // printEGLConfigAttributes(egl_dpy, config);



    return 0;
}

// Return a description of the specified EGL error
//
static const char* getEGLErrorString(EGLint error)
{
    switch (error)
    {
        case EGL_SUCCESS:
            return "Success";
        case EGL_NOT_INITIALIZED:
            return "EGL is not or could not be initialized";
        case EGL_BAD_ACCESS:
            return "EGL cannot access a requested resource";
        case EGL_BAD_ALLOC:
            return "EGL failed to allocate resources for the requested operation";
        case EGL_BAD_ATTRIBUTE:
            return "An unrecognized attribute or attribute value was passed in the attribute list";
        case EGL_BAD_CONTEXT:
            return "An EGLContext argument does not name a valid EGL rendering context";
        case EGL_BAD_CONFIG:
            return "An EGLConfig argument does not name a valid EGL frame buffer configuration";
        case EGL_BAD_CURRENT_SURFACE:
            return "The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid";
        case EGL_BAD_DISPLAY:
            return "An EGLDisplay argument does not name a valid EGL display connection";
        case EGL_BAD_SURFACE:
            return "An EGLSurface argument does not name a valid surface configured for GL rendering";
        case EGL_BAD_MATCH:
            return "Arguments are inconsistent";
        case EGL_BAD_PARAMETER:
            return "One or more argument values are invalid";
        case EGL_BAD_NATIVE_PIXMAP:
            return "A NativePixmapType argument does not refer to a valid native pixmap";
        case EGL_BAD_NATIVE_WINDOW:
            return "A NativeWindowType argument does not refer to a valid native window";
        case EGL_CONTEXT_LOST:
            return "The application must destroy all contexts and reinitialise";
        default:
            return "ERROR: UNKNOWN EGL ERROR";
    }
}

int load_egl() {
    printf("Loading EGL\n");
    // Ask for available extensions
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
	} else printf("Loaded extension eglGetPlatformDisplayEXT\n");


	eglCreatePlatformWindowSurfaceEXT =
		(PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
	if (eglCreatePlatformWindowSurfaceEXT == NULL) {
		fprintf(stderr, "Failed to get eglCreatePlatformWindowSurfaceEXT\n");
		return false;
	} else printf("Loaded extension eglCreatePlatformWindowSurfaceEXT\n");

    egl_dpy =  eglGetPlatformDisplayEXT(
            EGL_PLATFORM_WAYLAND_EXT, wl_dpy, NULL);
    if (egl_dpy == EGL_NO_DISPLAY) {
        printf("Failed to get EGL Display\n");
    }
    EGLint majVer, minVer;
    if (eglInitialize(egl_dpy, &majVer, &minVer) == EGL_FALSE) {
        printf("Failed to Init EGL\n");
        printf("%s\n",getEGLErrorString(eglGetError()));
    }
    printf("Got EGL %d.%d\n",majVer, minVer);

    return true;
}

const char* getEGLConfigAttributeName(EGLint attribute) {
    switch (attribute) {
        case EGL_BUFFER_SIZE:
            return "EGL_BUFFER_SIZE";
        case EGL_RED_SIZE:
            return "EGL_RED_SIZE";
        case EGL_GREEN_SIZE:
            return "EGL_GREEN_SIZE";
        case EGL_BLUE_SIZE:
            return "EGL_BLUE_SIZE";
        case EGL_ALPHA_SIZE:
            return "EGL_ALPHA_SIZE";
        case EGL_DEPTH_SIZE:
            return "EGL_DEPTH_SIZE";
        case EGL_STENCIL_SIZE:
            return "EGL_STENCIL_SIZE";
        case EGL_SAMPLES:
            return "EGL_SAMPLES";
        case EGL_SAMPLE_BUFFERS:
            return "EGL_SAMPLE_BUFFERS";
        case EGL_SURFACE_TYPE:
            return "EGL_SURFACE_TYPE";
        case EGL_RENDERABLE_TYPE:
            return "EGL_RENDERABLE_TYPE";
        case EGL_CONFIG_ID:
            return "EGL_CONFIG_ID";
        default:
            return "UNKNOWN ATTRIBUTE";
    }
}
void printEGLConfigAttributes(EGLDisplay egl_dpy, EGLConfig config) {
    EGLint attribs[] = {
        EGL_BUFFER_SIZE,
        EGL_RED_SIZE,
        EGL_GREEN_SIZE,
        EGL_BLUE_SIZE,
        EGL_ALPHA_SIZE,
        EGL_DEPTH_SIZE,
        EGL_STENCIL_SIZE,
        EGL_SAMPLES,
        EGL_SAMPLE_BUFFERS,
        EGL_SURFACE_TYPE,
        EGL_RENDERABLE_TYPE,
        EGL_CONFIG_ID,
        EGL_NONE
    };

    printf("EGL Configuration Attributes:\n");

    for (int i = 0; attribs[i] != EGL_NONE; i++) {
        EGLint value;
        if (eglGetConfigAttrib(egl_dpy, config, attribs[i], &value)) {
            printf("%s: %d\n", getEGLConfigAttributeName(attribs[i]), value);
        }
    }
}

