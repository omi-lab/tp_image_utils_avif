TARGET = tp_image_utils_avif
TEMPLATE = lib

DEFINES += TP_IMAGE_UTILS_AVIF_LIBRARY

SOURCES += src/Globals.cpp
HEADERS += inc/tp_image_utils_avif/Globals.h

SOURCES += src/LoadImages.cpp
HEADERS += inc/tp_image_utils_avif/LoadImages.h

SOURCES += src/AVIFSequence.cpp
HEADERS += inc/tp_image_utils_avif/AVIFSequence.h
