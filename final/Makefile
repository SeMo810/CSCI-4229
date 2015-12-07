# DETECT PLATFORM =============
# MinGW
ifeq "$(OS)" "Windows_NT"
  CFLAGS=-O3 -Wall -std=c++11 -Iexternal/include -Iinclude
  LIBS=-Lexternal/mingw -lglfw3 -lSOIL -lglu32 -lopengl32 -lwinmm -lgdi32
  LDFLAGS=-static-libgcc -static-libstdc++
  DEFINES=-DPLATFORM_WINDOWS
  MKDIR=md
  RMDIR=if exist "FOLDER" ( rd /s /q "FOLDER" )
  COPY=xcopy /s /e /i
else
# OSX
ifeq "$(shell uname)" "Darwin"
  CFLAGS=-O3 -Wall -std=c++11 -Iexternal/include -Iinclude -Wno-deprecated-declarations
  LIBS=-Lexternal/osx -framework glfw3 -framework glu32 -framework OpenGL
  DEFINES=-DPLATFORM_MAC
# Linux/Unix/Solaris
else
  CFLAGS=-O3 -Wall -std=c++11 -Iexternal/include -Iinclude
  LIBS=-Lexternal/linux -lglfw3 -lX11 -lXrandr -lXi -lXxf86vm -lXinerama -lXcursor -lrt -ldl -lpthread -lSOIL -lGLU -lGL -lm
  DEFINES=-DPLATFORM_LINUX
endif
  LDFLAGS=
  MKDIR=mkdir
  RMDIR=if [ -d "FOLDER" ]; then rm -rf "FOLDER"; fi
  COPY=cp -r
endif
# =============================

# Platform independent settings
CXX=g++
OBJ_DIR=obj
BIN_DIR=bin
EXEC=final

# A list of all of the object files to include
SOURCES=content.cpp \
					final.cpp \
					input.cpp \
					log.cpp \
					graphics/camera.cpp \
					graphics/debug.cpp \
					graphics/lighting.cpp \
					graphics/ogl.cpp \
					graphics/window_manager.cpp \
					thread/tinythread.cpp \
					world/peg.cpp \
					world/scripter.cpp \
					world/ship.cpp \
					world/world.cpp
SOURCES:=$(SOURCES:%=src/%)
OBJECTS=$(SOURCES:%.cpp=%.o)

# The command to make everything
all: dirs $(EXEC) content

# The executable builder
$(EXEC): $(OBJECTS)
	$(eval OBJFILES:=$(subst /,.,$(OBJECTS)))
	$(eval OBJFILES:=$(OBJFILES:%=$(OBJ_DIR)/%))
	$(CXX) $(OBJFILES) $(LIBS) $(LDFLAGS) -o $(BIN_DIR)/$(EXEC)

# The object files
%.o: %.cpp
	$(eval OBJFILE:=$(subst /,.,$@))
	$(eval OBJFILE:=$(OBJFILE:%=$(OBJ_DIR)/%))
	$(CXX) $(DEFINES) $(CFLAGS) -c $< -o $(OBJFILE)

# Copy the content
content: dirs
	$(COPY) "content" "bin/content"

# Create the output directories
dirs: clean
	$(MKDIR) $(OBJ_DIR)
	$(MKDIR) $(BIN_DIR)

# Clean the output
clean:
	$(subst FOLDER,$(OBJ_DIR),$(RMDIR))
	$(subst FOLDER,$(BIN_DIR),$(RMDIR))
