#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f hw5 *.o *.a
endif

#  Compile and link
hw5: hw5.c render/render_helper.c render/camera.c render/vector.c render/lighting.c
	gcc $(CFLG) -o hw5 $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
