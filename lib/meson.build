# GLAD

glad_src = ['glad/src/glad.c'] 
glad_include = include_directories('glad/include')

glad = static_library(
  'glad', 
  sources: glad_src,
  include_directories: glad_include
  )

# STB

stb_include = [include_directories('stb')]

# GLFW


wayland_egl = dependency('wayland-egl')
wayland_cursor = dependency('wayland-cursor')

wayland_client = dependency('wayland-client',
  version: '>=0.2.7')

egl=dependency('egl', required: false, disabler: true)


glfw_deps = [wayland_client, wayland_egl, wayland_cursor, egl]



## Wayland code/header generation

wayland_protos = dependency('wayland-protocols',
  version: '>=1.23',
  fallback: ['wayland-protocols', 'wayland_protocols'],
  default_options: ['tests=false'],
)

wayland_scanner_dep = dependency('wayland-scanner',
  native: true)

wl_protocol_dir = wayland_protos.get_variable('pkgdatadir')
wl_client_dir = wayland_client.get_variable('pkgdatadir')

wayland_scanner = find_program(
	wayland_scanner_dep.get_variable('wayland_scanner'),
    native: true
)

wayland_client = dependency('wayland-client')

protocols = {
  'wayland-xdg-shell': {
    'dir': wl_protocol_dir / 'stable/xdg-shell/xdg-shell.xml',
  },
  'wayland-wlr-layer-shell-unstable-v1' : {
    'dir': 'wlr-layer-shell-unstable-v1.xml',
  },
  'wayland-xdg-decoration': {
    'dir': wl_protocol_dir / 'unstable/xdg-decoration/xdg-decoration-unstable-v1.xml',
  },
  'wayland-viewporter': {
    'dir': wl_protocol_dir / 'stable/viewporter/viewporter.xml',
  },
  'wayland-relative-pointer-unstable-v1': {
    'dir': wl_protocol_dir / 'unstable/relative-pointer/relative-pointer-unstable-v1.xml',
  },
  'wayland-pointer-constraints-unstable-v1': {
    'dir': wl_protocol_dir / 'unstable/pointer-constraints/pointer-constraints-unstable-v1.xml'
  },
  'wayland-idle-inhibit-unstable-v1': {
    'dir': wl_protocol_dir / 'unstable/idle-inhibit/idle-inhibit-unstable-v1.xml'
  },
  'wayland' : {
    'dir': wl_client_dir / 'wayland.xml'
    }
}

## Generate headers from Wayland protocol XML files
protocols_code = []
foreach name, proto : protocols 
  code = custom_target(
    name + '_c',
    input: proto['dir'],
    output:  name + '-client-protocol-code.h',
    command: [
      wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@'
      ]
  )

  client_header = custom_target(
    name + '_client_h',
    input: proto['dir'],
    output: name + '-client-protocol.h',
    command: [
      wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'
      ]
  )
  protocols_code += code
  protocols_code += client_header
  # protocols_code += server_header
  # protocols_header += client_header
endforeach

glfw_dir = 'glfw/'

glfw_sources = [
  glfw_dir + 'include/GLFW/glfw3.h',
  glfw_dir + 'include/GLFW/glfw3native.h',
  glfw_dir + 'src/internal.h',
  glfw_dir + 'src/platform.h',
  glfw_dir + 'src/mappings.h',
  glfw_dir + 'src/context.c',
  glfw_dir + 'src/init.c',
  glfw_dir + 'src/input.c',
  glfw_dir + 'src/monitor.c',
  glfw_dir + 'src/platform.c',
  glfw_dir + 'src/vulkan.c',
  glfw_dir + 'src/window.c',
  glfw_dir + 'src/egl_context.c',
  glfw_dir + 'src/osmesa_context.c',
  glfw_dir + 'src/null_platform.h',
  glfw_dir + 'src/null_joystick.h',
  glfw_dir + 'src/null_init.c',
  glfw_dir + 'src/null_monitor.c',
  glfw_dir + 'src/null_window.c',
  glfw_dir + 'src/null_joystick.c',
  glfw_dir + 'src/posix_time.h',
  glfw_dir + 'src/posix_time.c',
  glfw_dir + 'src/posix_thread.h',
  glfw_dir + 'src/posix_thread.c',
  glfw_dir + 'src/posix_module.c',
  glfw_dir + 'src/wl_platform.h',
  glfw_dir + 'src/xkb_unicode.h',
  glfw_dir + 'src/wl_init.c',
  glfw_dir + 'src/wl_monitor.c',
  glfw_dir + 'src/wl_window.c',
  glfw_dir + 'src/xkb_unicode.c',
  glfw_dir + 'src/linux_joystick.h',
  glfw_dir + 'src/linux_joystick.c',
  glfw_dir + 'src/posix_poll.h',
  glfw_dir + 'src/posix_poll.c',
]


glfw_include = include_directories([glfw_dir + 'include'])

glfw = static_library('glfw', 
  sources: glfw_sources + protocols_code,
  include_directories: [glfw_dir + 'src'],
  c_args:['-D_GLFW_WAYLAND'],
  dependencies: glfw_deps)


## Graph2

subdir('graph2')

## NanoVGXC

nanovg_files = [
  'nanovgXC/src/fontstash.h',
  'nanovgXC/src/nanovg.h',
  'nanovgXC/src/nanovg_gl.h',
  'nanovgXC/src/nanovg_gl_utils.h',
  'nanovgXC/src/nanovg.c',
]

nanovg_include = include_directories('nanovgXC/src/')

nanovg = static_library('nanovg', 
  sources: nanovg_files,
  link_with: glad,
  include_directories: [nanovg_include, stb_include]
  )


## Final declarations

deps = [glfw, graph2, glad, nanovg]
deps_include = [glfw_include, graph2_include, glad_include,
  nanovg_include,]

