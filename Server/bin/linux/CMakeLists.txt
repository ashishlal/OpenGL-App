
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# add the executable
add_executable(OpenGLBroadcastServer ${PROJECT_SOURCE_DIR}/server/CCUDT.cpp  ${PROJECT_SOURCE_DIR}/server/CCOpenGLNew.cpp ${PROJECT_SOURCE_DIR}/server/CCJpeg.cpp ${PROJECT_SOURCE_DIR}/server/main.cpp)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/server/main.cpp PROPERTY COMPILE_DEFINITIONS BONJOUR_BIN_DIR="${PROJECT_BIN_DIR}/linux")

# -lGLEW -lglut -lstdc++ -ludt -lpthread -lm -ljpeg
target_link_libraries (OpenGLBroadcastServer ${GLEW})
target_link_libraries (OpenGLBroadcastServer ${GLUT})
target_link_libraries (OpenGLBroadcastServer "stdc++")
target_link_libraries (OpenGLBroadcastServer ${UDT})
target_link_libraries (OpenGLBroadcastServer ${PTHREAD})
target_link_libraries (OpenGLBroadcastServer ${JPEG})
#target_link_libraries (OpenGLBroadcastServer ${MAGICK})

install (TARGETS OpenGLBroadcastServer DESTINATION ${PROJECT_BIN_DIR})

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


