SHELL = cmd
CC = gcc
CLINKER = $(CC)
DEFINE = PRINT_WITH_COLOR

.PHONY: all
all: FrogJumpWalk_game.exe FrogJumpWalk_solve.exe

%.exe: %.obj
	@echo Linking $@ against $^ ...
	$(CLINKER) -o $@ $^

%.obj: %.c
	@echo Compiling $@ from $< ...
	$(CC) -o $@ -c $< -D $(DEFINE) -static -s

clean: clean
	-del /q *.exe 2> NUL

