project('predicates', 'cpp',
  version: '0.0.1',
  default_options : ['cpp_std=c++20'])

# Set up the dependencies and install targets
headers = ['include/predicates/predicates.hpp']
install_headers(headers, subdir: 'predicates')

include_dir = include_directories('include')
predicates_dep = declare_dependency(include_directories: include_dir)
dependencies = [dependency('eigen3'), dependency('boost')]

# Build the unit tests, benchmarks, and other demos
bench = executable('bench', 'test/bench.cpp',
  dependencies: [dependencies, predicates_dep])

unittest = executable('unittest', 'test/unittest.cpp',
  dependencies: [dependencies, predicates_dep])

test('benchmark', bench)
test('unittest', unittest)

# Create a Python wrapper
python = import('python').find_installation('python3')
python.extension_module('predicates',
  sources: ['src/pypredicates.cpp'],
  include_directories: include_dir,
  dependencies: [python.dependency(), dependency('pybind11'), dependencies],
)

# Generate a pkg-config file so other libraries can find us
pkg = import('pkgconfig')
pkg.generate(
  name: 'predicates',
  filebase: 'predicates',
  subdirs: ['predicates'],
  version: meson.project_version(),
  description: 'Sign-exact geometric predicates',
)
