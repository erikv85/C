fd: fd.o hamiltonian.o linalg.o plotfunc.o powermethod.o
	gcc fd.o hamiltonian.o linalg.o plotfunc.o powermethod.o -lm -lpthread -msse4.1 -O3 -o fd

fd.o: fd.c
	gcc -c fd.c

hamiltonian.o: hamiltonian.c
	gcc -c hamiltonian.c

linalg.o: linalg.c
	gcc -c linalg.c -msse4.1

plotfunc.o: plotfunc.c
	gcc -c plotfunc.c

powermethod.o: powermethod.c
	gcc -c powermethod.c

test:
	gcc fd.c hamiltonian.c linalg.c plotfunc.c powermethod.c -lm -lpthread -msse4.1 -o fd

clean:
	rm -f *.o fd *~
