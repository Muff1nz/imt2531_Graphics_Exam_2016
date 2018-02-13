#Files to be compiled
OBJS = main.cpp GameLevel.cpp InputHandler.cpp Mesh.cpp Camera.cpp Car.cpp MeshHandler.cpp GameObject.cpp globals.cpp lightHandler.cpp lights.cpp Mirror.cpp MirrorHandler.cpp ParticleSystem.cpp ShaderHandler.cpp TextureHandler.cpp

#Compiler
CC = g++

#Compiler flags
# -Wall 		Includes warnings
# -g			Include debug
# -ggdb			More debug info
# -O0			Turn off optimization
# -std=c++14	Use c++ 14
CFLAGS = -Wall -g -std=c++14 -ggdb

#Linker flags
# -lSDL2			Link to the SDL2 library
# -lSDL2_image		Link to the SDL2_image library, needed for more advanced image file types
LFLAGS = -lSDL2 -lSDL2_image -lGL -lGLU -lGLEW

#Name of our exectuable
OBJ_NAME = exam2016.exe

#This is the target that compiles our executable
#Run with command: "make"
all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(OBJ_NAME)

#Removes old compiled files. Usefull for running clean builds.
#Run with command: "make clean"
clean :
	rm *.o
	rm $(OBJ_NAME)
