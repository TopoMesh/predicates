project('predicates', 'cpp',
  version: '0.0.1',
  default_options : ['cpp_std=c++20'])

headers = ['include/predicates/predicates.hpp']
install_headers(headers, subdir : 'predicates')

include_dir = include_directories('include')
predicates_dep = declare_dependency(include_directories: include_dir)
dependencies = [
  dependency('eigen3'), dependency('boost')
]

bench = executable('bench', 'test/bench.cpp',
  dependencies: [dependencies, predicates_dep])

unittest = executable('unittest', 'test/unittest.cpp',
  dependencies: [dependencies, predicates_dep])

test('benchmark', bench)
test('unittest', unittest)
