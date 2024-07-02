OBJS = $(wildcard src/*.cpp) main.cpp
CC = g++
COMPILER_FLAGS = -g -w -I ./include
LINKER_FLAGS = -lSDL2 
OBJ_NAME = jumper


all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)