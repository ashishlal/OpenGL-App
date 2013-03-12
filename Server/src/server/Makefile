ifndef os
   os = LINUX
endif

PROJ_ROOT = `pwd`/../..

ifeq ($(os), WIN32)
CC = g++ -enable-auto-import -DWIN32 -D__MINGW__ -D_WIN32_WINNT=0x0501 


#LIBS = glu32.lib glew32.lib freeglut.lib glew32s.lib -lws2_32 -ludt -lopengl32 -lglu32 -lglew32 -lpthread-2 -lm -ljpeg
LIBS = glu32.lib glew32.lib freeglut.lib glew32s.lib -lws2_32 -ludt -lopengl32 -lglu32 -lglew32 -lpthread-2 -lm -ljpeg -lturbojpeg
LDFLAGS  = -L"C:\libjpeg-turbo-gcc\lib" -L"C:\libjpeg-turbo-gcc\bin" -L"C:\MinGW\bin"
INCLUDES = -I"C:\libjpeg-turbo-gcc\include" -I"C:\MinGW\include" 
LIBRARY = -L"C:\libjpeg-turbo-gcc\lib" -L"C:\libjpeg-turbo-gcc\bin" -L"C:\MinGW\lib" -L".\winlib"" 
CFLAGS   =  -O2 -ansi -Wall 
all: ServerWin.exe 
endif

# on linux create a symbolic link from /usr/lib/libudt.so or /usr/local/lib/libudt.so to the libudt.so
# file in the src directory

ifeq ($(os), LINUX)
CC = g++ -DLINUX
LIBS = -lGLEW -lglut -lstdc++ -ludt -lpthread -lm -ljpeg -lMagick++
CFLAGS   =  -O2 -ansi -ggdb -Wall 
INCLUDES = -I/opt/local/include -I/usr/local/include -I/usr/include/ImageMagick -I/usr/include/ImageMagick/Magick++
LDFLAGS = -L$(PROJ_ROOT)/lib/linux
all: ServerLinux
endif

ifeq ($(os), OSX)
CC = g++ -DOSX
# LIBS = -lglut -lgl -lglu -lstdc++ -ludt -lpthread -lm -lavcodec -lavformat -lavutil -lavcore  -lavfilter -lavdevice -lswscale
 LIBS = -lglut -lgl -lglu -lstdc++ -ludt -lpthread -lm -ljpeg
CFLAGS   =  -O2 -ansi -g -Wall 
INCLUDES = -I/usr/X11/include -I/opt/local/include -I/usr/local/include -I/usr/include/ImageMagick -I/usr/include/ImageMagick/Magick++
LDFLAGS = -L$(PROJ_ROOT)/lib/mac -L/opt/local/lib -L/usr/X11/lib -L/usr/local/lib 
all: ServerOSX
endif

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

ServerWin.exe: CCUDT.o  CCOpenGL.o CCJpeg.o main.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $^ $(LIBS)

ServerLinux: CCUDT.o  CCOpenGL.o CCJpeg.o  main.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $^ $(LIBS)
	
ServerOSX: CCUDT.o  CCOpenGL.o CCJpeg.o main.o
	$(CC) $(CFLAGS)  $(LDFLAGS) $^ $(LIBS) -o $@

clean:
	rm -rf ServerWin.exe ServerLinux ServerOSX *.o core 

.PHONY: clean
