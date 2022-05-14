SHELL = cmd
CC = gcc
CLINKER = $(CC)

.PHONY: all
all: FrogJumpWalk_game.exe FrogJumpWalk_solve.exe

%.exe: %.obj
	@echo Linking $@ against $^ ...
	$(CLINKER) -o $@ $^

%.obj: %.c
	@echo Compiling $@ from $< ...
	$(CC) -o $@ -c $< -static -s

clean: clean
	-del /q *.exe 2> NUL

