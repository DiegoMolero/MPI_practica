all:
	mpicc -o practica practica.c
	mpirun -np ${n} practica
