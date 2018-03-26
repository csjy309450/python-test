cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME gst_test2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp gtk_player.h mp3_player.h gtk_player.cpp)

## 寻找 gst pkg
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST QUIET gstreamer-1.0)
message(STATUS "GST library status:")
message(STATUS "    version: ${GST_VERSION}")
include_directories(${GST_INCLUDE_DIRS})

## 寻找 gtk-2.0 pkg
set(gkt2_include_dirs /usr/include/gtk-2.0 /usr/lib/x86_64-linux-gnu/gtk-2.0/include /usr/include/gio-unix-2.0/
        /usr/include/cairo /usr/include/pango-1.0 /usr/include/atk-1.0 /usr/include/cairo /usr/include/pixman-1
        /usr/include/libpng12 /usr/include/gdk-pixbuf-2.0 /usr/include/libpng12 /usr/include/pango-1.0
        /usr/include/harfbuzz /usr/include/pango-1.0 /usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include
        /usr/include/freetype2)
include_directories(${gkt2_include_dirs})
set(gtk2_libs )
find_library(gtk2_libs0 gtk-x11-2.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs1 gdk-x11-2.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs2 pangocairo-1.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs3 atk-1.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs4 cairo /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs5 gdk_pixbuf-2.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs6 gio-2.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs7 pangoft2-1.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs8 pango-1.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs9 gobject-2.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs10 glib-2.0 /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs11 fontconfig /usr/lib/x86_64-linux-gnu)
find_library(gtk2_libs12 freetype /usr/lib/x86_64-linux-gnu)
message(STATUS "gtk2 library status:")
message(STATUS "    version: 2.0")
list(APPEND gtk2_libs ${gtk2_libs0} ${gtk2_libs1} ${gtk2_libs2} ${gtk2_libs3}
        ${gtk2_libs4} ${gtk2_libs5} ${gtk2_libs6} ${gtk2_libs7} ${gtk2_libs8}
        ${gtk2_libs9} ${gtk2_libs10} ${gtk2_libs11} ${gtk2_libs12})

add_definitions(-fpermissive)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GST_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${gtk2_libs})