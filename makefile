all: lab6.c polyhedron.c
	gcc -o lab6 lab6.c polyhedron.c -lGL -lglut -lGLU -lm

clean: 
	$(RM) lab6.o
