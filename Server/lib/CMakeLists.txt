cmake_minimum_required(VERSION 2.8)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${PROJECT_LIB_DIR}/mac)

# -lglut -lgl -lglu -lstdc++ -ludt -lpthread -lm -ljpeg -lMagick++ -lMagickCore
find_library (GLUT glut VARIANT static ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(GL gl PATHS ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(GLU glu ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(UDT udt ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(PTHREAD pthread ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(M m ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(JPEG jpeg ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(MAGICK++ Magick++ ${USR_LOCAL_LIB_DIR})

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${PROJECT_LIB_DIR}/win)

# LIBS = glu32.lib glew32s.lib freeglut.lib  
#-lws2_32 -ludt -lopengl32 -lglu32 -lglew32 -lpthread-2 -lm -ljpeg -lturbojpeg

# find_library(GLU glu32.lib ${PROJECT_LIB_DIR}/win})
# find_library(GLEW32s glew32s.lib ${PROJECT_LIB_DIR}/win})
# find_library(FREEGLUT freeglut.lib ${PROJECT_LIB_DIR}/win})

find_library(WS232 ws2_32 ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
find_library(UDT udt ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
find_library(OPENGL32 opengl32 ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
find_library(LGLU32 glu32  ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
find_library(LGLEW32 glew32 ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
find_library(LPTHREAD2 pthread-2 ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
find_library(LM m  ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
#find_library(JPEG jpeg ${TURBO_JPEG_DIR}/lib ${TURBO_JPEG_DIR}/bin ${MINGW_LIB_DIR} ${MINGW_BIN_DIR})
find_library(JPEG jpeg-80 ${USR_LOCAL_LIB_DIR})
find_library(TJPEG turbojpeg ${TURBO_JPEG_DIR}/lib ${TURBO_JPEG_DIR}/bin ${MINGW_LIB_DIR})
#find_library(MAGICKPP Magick++ "/usr/local/lib")
#find_library(LTDLC ltdlc "/usr/local/lib")
#find_library(MC MagickCore "/usr/local/lib")
#find_library(MW MagickWand "/usr/local/lib")


ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# -lGLEW -lglut -lstdc++ -ludt -lpthread -lm -ljpeg -lMagick++

find_library(GLEW GLEW ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(GLUT glut ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(UDT udt ${PROJECT_LIB_DIR}/linux ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(PTHREAD pthread ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(M m ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
find_library(JPEG jpeg ${OPT_LOCAL_LIB_DIR} ${USR_X11_LIB_DIR} ${USR_LOCAL_LIB_DIR})
#find_library(MAGICK Magick++ ${OPT_LOCAL_LIB_DIR} ${USR_LIB_DIR} ${USR_LOCAL_LIB_DIR})

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
