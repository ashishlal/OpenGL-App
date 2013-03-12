# Install script for directory: /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac/OpenGLBroadcastServer")
FILE(INSTALL DESTINATION "/Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac" TYPE EXECUTABLE FILES "/Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac/OpenGLBroadcastServer")
  IF(EXISTS "$ENV{DESTDIR}/Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac/OpenGLBroadcastServer" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac/OpenGLBroadcastServer")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac/OpenGLBroadcastServer")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

