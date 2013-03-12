 
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# add the executable
set_property(SOURCE ${PROJECT_SOURCE_DIR}/server/main.cpp PROPERTY COMPILE_DEFINITIONS BONJOUR_BIN_DIR="${PROJECT_BIN_DIR}/mac")
#add_executable(OpenGLBroadcastServer ${PROJECT_SOURCE_DIR}/server/CCUDT.cpp  ${PROJECT_SOURCE_DIR}/server/CCOpenGL.cpp ${PROJECT_SOURCE_DIR}/server/CCJpeg.cpp ${PROJECT_SOURCE_DIR}/server/main.cpp)
add_executable(OpenGLBroadcastServer ${PROJECT_SOURCE_DIR}/server/CCUDT.cpp  ${PROJECT_SOURCE_DIR}/server/CCOpenGLNew.cpp ${PROJECT_SOURCE_DIR}/server/CCJpeg.cpp ${PROJECT_SOURCE_DIR}/server/main.cpp)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/server/main.cpp PROPERTY COMPILE_DEFINITIONS BONJOUR_BIN_DIR="${PROJECT_BIN_DIR}/mac")
add_dependencies(OpenGLBroadcastServer BonjourServer.app)

set_target_properties(OpenGLBroadcastServer PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(OpenGLBroadcastServer PROPERTIES INSTALL_RPATH "${PROJECT_LIB_DIR}/mac/libudt.dylib")

add_custom_command(
  TARGET OpenGLBroadcastServer
  PRE_BUILD
  COMMAND sudo cp ${PROJECT_LIB_DIR}/mac/libudt.dylib "/usr/local/lib"
)

#COMMAND source ${PROJECT_LIB_DIR}/mac/exportUDT.sh ${PROJECT_LIB_DIR}/mac
#SET(ENV(DYLD_LIBRARY_PATH) ${PROJECT_LIB_DIR}/mac)

# -lglut -lgl -lglu -lstdc++ -ludt -lpthread -lm -ljpeg
target_link_libraries (OpenGLBroadcastServer "GLUT")
target_link_libraries (OpenGLBroadcastServer ${GL})
target_link_libraries (OpenGLBroadcastServer ${GLU})
target_link_libraries (OpenGLBroadcastServer "stdc++")
target_link_libraries (OpenGLBroadcastServer ${UDT})
target_link_libraries (OpenGLBroadcastServer ${PTHREAD})
target_link_libraries (OpenGLBroadcastServer ${M})
target_link_libraries (OpenGLBroadcastServer "jpeg")
#target_link_libraries (OpenGLBroadcastServer ${MAGICK++})

install (TARGETS OpenGLBroadcastServer DESTINATION ${PROJECT_BIN_DIR}/mac)

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

