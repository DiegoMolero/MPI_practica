#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void createRandomVector(int *array, int size){
        srand(time(NULL) + getpid());
        int numero = 0;
        int N = 65;
        int M = 0;
        int j = 0;
        //printf("Array inicial: ");
        for (j; j < size; j++ ) {
                // numero generando entre M y N
                numero = rand () % (N-M+1) + M;
                array[j] = numero;
                //printf("%d,",array[j]);
        }
        printf("\n");
}


int main(int argc, char** argv) {
        int n_processes, rank;
        int buf;
        const int root=0;
        double start, finish;
        //Array
        const int totalsize=1000000;
        int array[totalsize];

        //MPI
        MPI_Init(&argc, &argv);
        //Set MPI variables
        MPI_Comm_size(MPI_COMM_WORLD, &n_processes);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
        if(rank == root) {
                createRandomVector(array,totalsize);
                //Iniciar contador de tiempo
                start = MPI_Wtime();
        }
        //----------------------Diseño de FOSTER--------------------
        //----------------------------------------------------------
        //-------------------------PARTICIONAR---------------------- 
        int size_local = totalsize/n_processes;
        if(rank ==  n_processes-1){
                int resto = totalsize % size_local;
                size_local = size_local + resto;
        }
        int array_local[size_local];
        //----------------------COMUNICACION-----------------------
        MPI_Scatter(array, size_local, MPI_INT, array_local, size_local, MPI_INT, 0, MPI_COMM_WORLD); 
        //Procesar
        printf("El proceso %d tiene una array de size: %d\n", rank,size_local);
        int i=0;
        for(i;i<size_local;i++){
                if(array_local[i] <= 14){
                        array_local[i] = 1;
                }else if (array_local[i] <= 24)
                {
                        array_local[i] = 2;
                }else
                {
                array_local[i]=3;
                }        
        }
        //--------------------AGRUPAR Y ASIGNA-------------------
        MPI_Gather(array_local, size_local, MPI_INT, array, size_local, MPI_INT, 0, MPI_COMM_WORLD);

        //Mostrar resultados
        if(rank == root){
                /*
                i = 0;
                printf("\nResultado final:");
                for ( i; i < totalsize; i++)
                {
                        printf("%d,",array[i]);
                }
                */
                //Finalizar contador de tiempo
                finish = MPI_Wtime();
                printf("\nTiempo máximo de ejecución= %f segundos.\n", finish - start);  
        }     
        MPI_Finalize();

        return 0;
}