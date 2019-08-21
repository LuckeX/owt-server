# # Note: plugindir is set in configure

# ##############################################################################
# # TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
# ##############################################################################
# plugin_LTLIBRARIES = libgstplugin.la libgstaudiofilterexample.la

# ##############################################################################
# # TODO: for the next set of variables, name the prefix if you named the .la, #
# #  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
# #                            libmysomething_la_CFLAGS                        #
# #                            libmysomething_la_LIBADD                        #
# #                            libmysomething_la_LDFLAGS                       #
# ##############################################################################

# ## Plugin 1

# # sources used to compile this plug-in
# libgstplugin_la_SOURCES = gstplugin.c gstplugin.h

# # compiler and linker flags used to compile this plugin, set in configure.ac
# libgstplugin_la_CFLAGS = $(GST_CFLAGS)
# libgstplugin_la_LIBADD = $(GST_LIBS)
# libgstplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
# libgstplugin_la_LIBTOOLFLAGS = --tag=disable-static

# ## Plugin 2 (audio filter example)

# # sources used to compile this plug-in
# libgstaudiofilterexample_la_SOURCES = gstaudiofilter.c

# # compiler and linker flags used to compile this plugin, set in configure.ac
# libgstaudiofilterexample_la_CFLAGS = $(GST_CFLAGS)
# libgstaudiofilterexample_la_LIBADD = $(GST_LIBS)
# libgstaudiofilterexample_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
# libgstaudiofilterexample_la_LIBTOOLFLAGS = --tag=disable-static

# # headers we need but don't want installed
# noinst_HEADERS = gstplugin.h


plugin_LTLIBRARIES = libgstsendsink.la

libgstsendsink_la_SOURCES = gstsendsink.c \
                            case/wrapper.h \
                            case/wrapper.cpp \
                            case/RawTransport.h \
                            case/RawTransport.cpp \
                            case/logger.h 
	
libgstsendsink_la_CFLAGS = $(GST_CFLAGS) $(ZMQ_CFLAGS)
libgstsendsink_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)

libgstsendsink_la_LIBADD = $(GST_BASE_LIBS) $(GST_LIBS) 
libgstsendsink_la_LIBADD += -lpthread -lboost_thread -lboost_system -llog4cxx

libgstsendsink_la_LIBTOOLFLAGS = $(GST_PLUGIN_LIBTOOLFLAGS)

noinst_HEADERS = gstsendsink.h 
  


CLEANFILES = $(BUILT_SOURCES)
