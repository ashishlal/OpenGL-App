cmake_minimum_required(VERSION 2.8) 

project(OpenGLBroadcastServer)

#The version number.
set (OpenGLBroadcastServer_VERSION_MAJOR 1)
set (OpenGLBroadcastServer_VERSION_MINOR 0)


IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set (PROJECT_ROOT_DIR "/Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server")
set (PROJECT_SOURCE_DIR "${PROJECT_ROOT_DIR}/src")
set (PROJECT_LIB_DIR "${PROJECT_ROOT_DIR}/lib")
set (PROJECT_BIN_DIR "${PROJECT_ROOT_DIR}/bin")
#set (CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set (CMAKE_SKIP_BUILD_RPATH TRUE)
set (CMAKE_INSTALL_RPATH "${PROJECT_LIB_DIR}/mac/libudt.dylib")
#set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

MESSAGE ("Looks like a Darwin System..")

set (CMAKE_CXX_COMPILER "/usr/bin/g++")
set (USR_LOCAL_LIB_DIR "/usr/local/lib")
set (OPT_LOCAL_LIB_DIR "/opt/local/lib")
set (USR_LIB_DIR "/usr/lib")
set (USR_X11_LIB_DIR "/usr/X11/lib")
set (USR_X11_INCLUDE_DIR "/usr/X11/include")
set (OPT_INCLUDE_DIR "/opt/local/include")
set (IMAGEMAGICK_INCLUDE_DIR "/usr/local/include/ImageMagick")
set (IMAGEMAGICK_LIB_DIR "/usr/local/lib/ImageMagick")
set (USR_LOCAL_INCLUDE_DIR "/usr/local/include")

include_directories ("${PROJECT_SOURCE_DIR}/server")
include_directories ("${USR_X11_INCLUDE_DIR}")
include_directories ("${OPT_INCLUDE_DIR}")
include_directories ("${USR_LOCAL_INCLUDE_DIR}")
include_directories ("${USR_LOCAL_LIB_DIR}")
include_directories ("${USR_X11_LIB_DIR}")
include_directories ("${OPT_LOCAL_LIB_DIR}")
include_directories ("${USR_LIB_DIR}")
include_directories("${PROJECT_LIB_DIR}/mac")
include_directories("${PROJECT_BIN_DIR}/mac")
include_directories("${IMAGEMAGICK_INCLUDE_DIR}")

SET(CMAKE_CXX_FLAGS "-DOSX -O2 -ansi -ggdb -Wall -fasm-blocks")
SET(CMAKE_EXE_LINKER_FLAGS "-rpath,${PROJECT_LIB_DIR}/mac -L${PROJECT_LIB_DIR}/mac -L/opt/local/lib -L/usr/X11/lib -L/usr/local/lib")

add_subdirectory(${PROJECT_SOURCE_DIR}/bonjour/mac)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

MESSAGE ("Looks like a Windows System..")

set (PROJECT_ROOT_DIR "C:/lal/OpenGL-Broadcast-App-Maintenance/Server")
set (PROJECT_SOURCE_DIR "${PROJECT_ROOT_DIR}/src")
set (PROJECT_LIB_DIR "${PROJECT_ROOT_DIR}/lib")
set (PROJECT_BIN_DIR "${PROJECT_ROOT_DIR}/bin")

set (MINGW_LIB_DIR "C:/MinGW/lib")
set (MINGW_BIN_DIR "C:/MinGW/bin")
set (MINGW_INCLUDE_DIR "C:/MinGW/include")
set (WIN_SYSTEM_DIR "C:/Windows/system32")
set (TURBO_JPEG_DIR "C:/libjpeg-turbo-gcc")
set (MAGICK_INCLUDE_DIR "C:/MinGW/include/ImageMagick")
set (USR_LOCAL_LIB_DIR "/usr/local/lib")
set (USR_LOCAL_BIN_DIR "/usr/local/bin")
set (USR_LOCAL_INCLUDE_DIR "/usr/local/include")

include_directories ("${PROJECT_SOURCE_DIR}/server")
include_directories ("${USR_LOCAL_INCLUDE_DIR}")
include_directories ("${USR_LOCAL_LIB_DIR}")
include_directories ("${USR_LOCAL_BIN_DIR}")
include_directories ("${MINGW_LIB_DIR}")
include_directories ("${MINGW_BIN_DIR}")
include_directories ("${MINGW_INCLUDE_DIR}")
include_directories("${PROJECT_BIN_DIR}/win")
include_directories("${PROJECT_LIB_DIR}/lib")
include_directories("${PROJECT_LIB_DIR}/lib/win")
include_directories("${TURBO_JPEG_DIR}/lib")
include_directories("${TURBO_JPEG_DIR}/include")
include_directories("${TURBO_JPEG_DIR}/bin")
include_directories("${MAGICK_INCLUDE_DIR}")

#link_directories("${PROJECT_LIB_DIR}/win")
SET(CMAKE_CXX_FLAGS "-enable-auto-import -enable-stdcall-fixup -DWIN32 -D__MINGW__ -D_WIN32_WINNT=0x0501  -O2 -ggdb -ansi -Wall")
SET(CMAKE_EXE_LINKER_FLAGS "-L${TURBO_JPEG_DIR}/lib -L${TURBO_JPEG_DIR}/bin -L/usr/local/lib -L/usr/local/bin -L${MINGW_BIN_DIR}")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
MESSAGE ("Looks like a Linux System..")

set (PROJECT_ROOT_DIR "/home/ashish/OpenGL-Broadcast-App-Maintenance/Server")
set (PROJECT_SOURCE_DIR "${PROJECT_ROOT_DIR}/src")
set (PROJECT_LIB_DIR "${PROJECT_ROOT_DIR}/lib")
set (PROJECT_BIN_DIR "${PROJECT_ROOT_DIR}/bin")

set (USR_LOCAL_LIB_DIR "/usr/local/lib")
set (OPT_LOCAL_LIB_DIR "/opt/local/lib")
set (USR_LIB_DIR "/usr/lib")
set (USR_X11_INCLUDE_DIR "/usr/X11/include")
set (OPT_INCLUDE_DIR "/opt/local/include")
set (USR_LOCAL_INCLUDE_DIR "/usr/local/include")
set (USR_INCLUDE_DIR "/usr/include")
set (OPT_IMAGEMAGICK_INCLUDE_DIR "/opt/local/include/ImageMagick")
set (OPT_IMAGEMAGICK_LIB_DIR "/opt/local/lib/ImageMagick")
set (USR_IMAGEMAGICK_INCLUDE_DIR "/usr/include/ImageMagick")
set (USR_IMAGEMAGICK_LIB_DIR "/usr/lib/ImageMagick")

include_directories ("${PROJECT_SOURCE_DIR}/server")
include_directories ("${USR_X11_INCLUDE_DIR}")
include_directories ("${OPT_INCLUDE_DIR}")
include_directories ("${USR_LOCAL_INCLUDE_DIR}")
include_directories ("${USR_LOCAL_LIB_DIR}")
include_directories ("${OPT_LOCAL_LIB_DIR}")
include_directories ("${USR_LIB_DIR}")
include_directories ("${USR_IMAGEMAGICK_INCLUDE_DIR}")
include_directories("${PROJECT_LIB_DIR}/linux")
include_directories("${PROJECT_BIN_DIR}/linux")

SET(CMAKE_CXX_FLAGS "-DLINUX -O2 -ansi -Wall")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

add_subdirectory (lib) 
add_subdirectory (bin) 



