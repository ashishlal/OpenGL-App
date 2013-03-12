IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
# add the executable
add_executable(OpenGLBroadcastServer ${PROJECT_SOURCE_DIR}/server/CCUDT.cpp  ${PROJECT_SOURCE_DIR}/server/CCOpenGLNew.cpp ${PROJECT_SOURCE_DIR}/server/CCJpeg.cpp ${PROJECT_SOURCE_DIR}/server/main.cpp)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/server/main.cpp PROPERTY COMPILE_DEFINITIONS BONJOUR_BIN_DIR="${PROJECT_BIN_DIR}/win")

# LIBS = glu32.lib glew32s.lib freeglut.lib -lws2_32 -ludt -lopengl32 -lglu32 -lglew32 -lpthread-2 -lm -ljpeg -lturbojpeg

#target_link_libraries (OpenGLBroadcastServer ${GLU})
#target_link_libraries (OpenGLBroadcastServer ${GLEW32s})
#target_link_libraries (OpenGLBroadcastServer ${FREEGLUT})

target_link_libraries (OpenGLBroadcastServer ${PROJECT_LIB_DIR}/win/GlU32.lib)
#target_link_libraries (OpenGLBroadcastServer ${PROJECT_LIB_DIR}/win/glew32s.lib)
target_link_libraries (OpenGLBroadcastServer ${PROJECT_LIB_DIR}/win/freeglut.lib)

target_link_libraries (OpenGLBroadcastServer ${WS232})
target_link_libraries (OpenGLBroadcastServer ${UDT})
target_link_libraries (OpenGLBroadcastServer ${OPENGL32})
target_link_libraries (OpenGLBroadcastServer ${LGLU32})
target_link_libraries (OpenGLBroadcastServer "glew32")
#target_link_libraries (OpenGLBroadcastServer "Magick++")
#target_link_libraries (OpenGLBroadcastServer "MagickWand")
#target_link_libraries (OpenGLBroadcastServer "MagickCore")
#target_link_libraries (OpenGLBroadcastServer "ltdlc")
target_link_libraries (OpenGLBroadcastServer ${JPEG})
target_link_libraries (OpenGLBroadcastServer ${TJPEG})
target_link_libraries (OpenGLBroadcastServer ${LPTHREAD2})
target_link_libraries (OpenGLBroadcastServer "z")
target_link_libraries (OpenGLBroadcastServer "m")
target_link_libraries (OpenGLBroadcastServer "gomp")

install (TARGETS OpenGLBroadcastServer DESTINATION ${PROJECT_BIN_DIR}/win)

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

