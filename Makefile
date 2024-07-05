OBJS = $(wildcard src/*.cpp)
CC = g++
COMPILER_FLAGS = -g -w -I ./include
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer
OBJ_NAME = jumper


all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)