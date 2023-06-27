project('layer_shell_example', 'c', 'cpp')

subdir('lib')

# Source files
src = files(
  'main.cpp',
  'src/clock.cpp',
  'src/view.cpp',
  'src/demo.c',
)

# Include folder
include_dir = [include_directories('include'), ]

# Executable
exe = executable('layer_shell_example', 
  src,
  link_with: deps,
  include_directories: [include_dir] + deps_include,
  cpp_args: ['-g']
)
