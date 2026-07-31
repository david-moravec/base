project('base-lib', 'c')

inc = include_directories('include', 'utest')


host_system = host_machine.system()

if host_system == 'windows'
  platform_src = files('memory/win32_win64_memory.c')
elif host_system == 'linux'
  platform_src = files('memory/linux_memory.c')
else
  error('Unsupported platform: ' + host_system)
endif

lib_sources = files(
  'arena/arena.c',
  'collections/list.c',
) + platform_src

lib = static_library('myLib', lib_sources, include_directories: inc)

foreach t : ['test_list']
  exe = executable(t,
    'tests/' + t + '.c',
    include_directories : inc,
    link_with : lib
  )
  test(t, exe)
endforeach
