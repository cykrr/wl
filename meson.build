project('layer_shell_example', 'c', 'cpp')

# Source files
src = files(
  'main.cpp',
  'src/egl_common.cpp',
  'src/glad_egl.c',
  'src/glad.c'
)

# Include folder
include_dir = [include_directories('include'), include_directories('.')]

wayland_egl = dependency('wayland-egl')
wayland_cursor = dependency('wayland-cursor')
wayland_client = dependency('wayland-client', version: '>=0.2.7')
libdrm = dependency('libdrm').partial_dependency(compile_args: true, includes: true)

wayland_protos = dependency('wayland-protocols',
	version: '>=1.23',
	fallback: ['wayland-protocols', 'wayland_protocols'],
	default_options: ['tests=false'],
)
wl_protocol_dir = wayland_protos.get_variable('pkgdatadir')
wl_client_dir = wayland_client.get_variable('pkgdatadir')

wlroots=dependency('wlroots')
# glesv2=dependency('glesv2', required: false, disabler: true)
egl=dependency('egl', required: false, disabler: true)


wayland_scanner_dep = dependency('wayland-scanner', native: true)
wayland_scanner = find_program(
	wayland_scanner_dep.get_variable('wayland_scanner'),
    native: true
)

version = '1.0.0'


# Dependencies
wayland_client = dependency('wayland-client')


# Wayland protocol files
protocol_files = files(
  'wlr-layer-shell-unstable-v1.xml',
  # Add other Wayland protocol XML files as needed
)

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


protocols_code = []
# Generate headers from Wayland protocol XML files
foreach name, proto : protocols 
  code = custom_target(
    name + '_c',
    input: proto['dir'],
    output:  name + '-client-protocol-code.h',
    command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
  )

  client_header = custom_target(
    name + '_client_h',
    input: proto['dir'],
    output: name + '-client-protocol.h',
    command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@']
  )
  protocols_code += code
  protocols_code += client_header
  # protocols_code += server_header
  # protocols_header += client_header
endforeach

# code = custom_target(
#   'wayland' + '_c',
#   input: wl_client_dir + '/wayland.xml',
#   output: 'wayland-client-protocol.c',
#   command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
# )
#
# server_header = custom_target(
#   'wayland' + '_server_h',
#   input: wl_dir + '/wayland.xml',
#   output: 'wayland-server-protocol.h',
#   command: [wayland_scanner, 'server-header', '@INPUT@', '@OUTPUT@']
# )
#
# client_header = custom_target(
#    'wayland' + '_client_h',
#   input: wl_dir + '/wayland.xml',
#   output: 'wayland-client-protocol-code.h',
#   command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@']
# )

# protocols_code = protocols_code + code + server_header + client_header
# protocols_code += code

glfw_dir = 'lib/glfw/'

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

glfw_header = [
]
glfw = static_library('glfw', sources: glfw_header + glfw_sources + protocols_code, include_directories: [glfw_dir + 'src'] +  include_dir, c_args:['-D_GLFW_WAYLAND'], dependencies: [wayland_client, wayland_egl, wayland_cursor, egl])




subdir('lib/graph2')


# Executable
exe = executable('layer_shell_example', 
  src + protocols_code + fonts_files,
  link_with: glfw,
  include_directories: [include_dir, fonts_include, glfw_dir + 'include'],
  cpp_args: ['-g']
)
