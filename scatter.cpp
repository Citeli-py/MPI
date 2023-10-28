#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define Debug true

void print(int *C, int n, int m){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%d\t",C[i*m + j]); 
		}
		printf("\n");
	}
}


int main(int argc , char *argv[])
{
	int tam = atoi(argv[1]);
	int size,rank,sum=0,i,j,k;
	int *A, *B, *C;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// criando um array de ponteiros
	A =(int *)malloc(tam *tam * sizeof(int));
	B =(int *)malloc(tam *tam * sizeof(int));
	C =(int *)malloc(tam *tam * sizeof(int));

	
    if(rank == 0)  // inicializando as matrizes com valor i+j
        for(i = 0; i < tam; i++)
            for(j = 0; j < tam; j++) {
                A[i*tam + j] = i+j;
                B[i*tam + j] = i+j;
				C[i*tam + j] = 0;
            }

	// Todos recebem B
	MPI_Bcast(B, tam * tam, MPI_INT, 0, MPI_COMM_WORLD);

	//Matrizes locais
	int localSize = tam/size;
	int *localA, *localRes;

	localA = (int*) malloc(tam*localSize*sizeof(int)); 
	localRes = (int*) malloc(tam*localSize*sizeof(int)); 

	// Divide a matriz A em sub-matrizes locais
    MPI_Scatter(A, tam * localSize, MPI_INT, localA, tam * localSize, MPI_INT, 0, MPI_COMM_WORLD);


	// dividindo as tarefas nos vários processos
	int start = (rank)*localSize;
	int end = (rank+1)*localSize;

	for(i = start; i < end; i++) // Linhas
		for(j = 0; j < tam; j++) // Colunas
		{
			sum=0;
			for(k = 0; k < tam; k++)
				sum += localA[(i-start)*tam + k] * B[k*tam + j];
		
			localRes[(i-start)*tam + j] = sum; 
		}

	// Recolha os resultados parciais de cada processo
    MPI_Gather(localRes, localSize * tam, MPI_INT, C, localSize*tam, MPI_INT, 0, MPI_COMM_WORLD);


	if((rank == 0) && Debug){
		print(C, tam, tam);
	}

	// Finaliza
	MPI_Finalize();
	return 0;
}