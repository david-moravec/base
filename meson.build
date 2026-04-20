project('base-lib', 'c')

inc = include_directories('arena', 'collections', 'utest')

lib_sources = files(
  'arena/arena.c',
  'collections/list.c'
)
lib = static_library('myLib', lib_sources, include_directories: inc)

foreach t : ['test_list']
  exe = executable(t,
    'tests/' + t + '.c',
    include_directories : inc,
    link_with : lib
  )
  test(t, exe)
endforeach
