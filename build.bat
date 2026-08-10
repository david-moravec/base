meson setup builddir --buildtype=release --native-file=clang.ini
cd builddir
meson compile
