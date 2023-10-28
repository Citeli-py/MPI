#include <stdio.h>
#include <stdlib.h>

#define Debug false

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
	int sum=0,i,j,k;
	int *A, *B, *C;

	// criando um array de ponteiros
	A =(int *)malloc(tam *tam * sizeof(int));
	B =(int *)malloc(tam *tam * sizeof(int));
	C =(int *)malloc(tam *tam * sizeof(int));

    // inicializando as matrizes com valor i+j
    for(i = 0; i < tam; i++)
    {
        for(j = 0; j < tam; j++)
        {
            A[i*tam + j] = i+j;
            B[i*tam + j] = i+j;
            C[i*tam + j] = 0;
        }
    }
    


	for(i = 0; i < tam; i++) // Linhas
	{
		for(j = 0; j < tam; j++) // Colunas
		{
			sum=0;
			for(k = 0; k < tam; k++)
			{
				sum += A[i*tam + k] * B[k*tam + j];
			}
			C[i*tam + j] = sum;
		}
	}

	if(Debug){
		print(C, tam, tam);
	}

	return 0;
}