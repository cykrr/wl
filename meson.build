project('layer_shell_example', 'c', 'cpp')

subdir('lib')

# Source files
src = files(
  'main.cpp',
  # 'src/egl_common.cpp',
)

# Include folder
include_dir = [include_directories('include'), ]

# Executable
exe = executable('layer_shell_example', 
  src,
  link_with: deps,
  include_directories: [include_dir, glad_include],
  cpp_args: ['-g']
)

# glesv2=dependency('glesv2', required: false, disabler: true)


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

