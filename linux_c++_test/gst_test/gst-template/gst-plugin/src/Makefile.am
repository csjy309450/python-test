# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstcamerasrc.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

## Plugin 1

# sources used to compile this plug-in
libgstcamerasrc_la_SOURCES = ./gstcamerasrc.c ./gstcamerasrc.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstcamerasrc_la_CFLAGS = $(GST_CFLAGS)
libgstcamerasrc_la_LIBADD = $(GST_LIBS)
libgstcamerasrc_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstcamerasrc_la_LIBTOOLFLAGS = --tag=disable-static
