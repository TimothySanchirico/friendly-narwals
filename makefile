CC=g++
CPP_FLAGS=-std=c++17 -Werror -g3
LD_FLAGS= -lGL -lSOIL -lGLU -lglfw -lGLEW
INCLUDE_DIRS=-I.

Main_exe: Main.C
	$(CC) $(LD_FLAGS) $(CPP_FLAGS) $(INCLUDE_DIRS) $^ -o $@

clean:
	rm Main_exe

