# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
##############################################################################
# plugin_LTLIBRARIES = libgstplugin.la libgstaudiofilterexample.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

## Plugin 1

# sources used to compile this plug-in
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



#############################################################################
# INCLUDES= -I./case/include
# plugin_LTLIBRARIES = libgstwebrtc.la

# libgstwebrtc_la_SOURCES = gstwebrtc.c  \
#                                  case/wrapper.cpp \
#                                  case/wrapper.h \
#                                  case/p2psocketsignalingchannel.h \
#                                  case/p2psocketsignalingchannel.cc

# libgstwebrtc_la_CFLAGS = $(GST_CFLAGS)

# libgstwebrtc_la_LIBADD = $(GST_LIBS) 
# # libgstwebrtc_la_LIBADD += -lboost_date_time \
# #         -lboost_random \
# #         -lboost_system \
# #         -lssl \
# #         -lsioclient \
# #         ./case/libs/libowt.a \
# #         -lcrypto \
# #         -lz \
# #         -lasound \
# #         -lbz2 \
# #         -lrt \
# #         -lm \
# #         -ldl \
# #         -lX11 \
# #         -lXext \
# #         -lXcomposite \
# #         -lXrender \
# #         -lpthread \
# #         -lva \
# #         -lva-drm \
# #         -lva-x11 \
# #         -lyami
#         # -lmfxhw64 

# libgstwebrtc_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
# libgstwebrtc_la_LIBTOOLFLAGS = --tag=disable-static

# noinst_HEADERS = gstwebrtc.h 


#######################################################
# INCLUDES= -I./case/include
# plugin_LTLIBRARIES = libgstanalytics.la

# libgstanalytics_la_SOURCES = gstanalytics.c \
#                              case/wrapper.cpp \
#                              case/wrapper.h \
#                              case/myplugin.h \
#                              case/myplugin.cc

# libgstanalytics_la_CFLAGS = $(GST_CFLAGS)

# libgstanalytics_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
# # libgstanalytics_la_LDFLAGS += -L./case/libs

# libgstanalytics_la_LIBADD = $(GST_LIBS) 
# # libgstanalytics_la_LIBADD += -lmyplugin


# libgstanalytics_la_LIBTOOLFLAGS = --tag=disable-static

# noinst_HEADERS = gstanalytics.h 

#########################################################
INCLUDES= -I./include
plugin_LTLIBRARIES = libgstfacerecognition.la

libgstfacerecognition_la_SOURCES = gstfacerecognition.c \
                             caseFaceRecognition/wrapper.cpp \
                             caseFaceRecognition/wrapper.h \
                             caseFaceRecognition/myplugin.h \
                             caseFaceRecognition/myplugin.cc
                            

libgstfacerecognition_la_CFLAGS = $(GST_CFLAGS)

libgstfacerecognition_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
# libgstfacerecognition_la_LDFLAGS += -L/home/xyk/Documents/owt-server/source/agent/plugins/samples/build/intel64/Release/lib
# libgstfacerecognition_la_LDFLAGS += -L/home/webrtc/Documents/plugin_lib

libgstfacerecognition_la_LIBADD = $(GST_LIBS) 
# libgstfacerecognition_la_LIBADD += -lmyplugin


libgstfacerecognition_la_LIBTOOLFLAGS = --tag=disable-static

noinst_HEADERS = gstfacerecognition.h 