#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 1000
void main(int argc, char *argv) {
	int myid, numprocs;
	int  i, x, myresult, result;
	int A[MAXSIZE];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if(myid == 0) for(i=0;i<MAXSIZE;i++) A[i]=i;
		
	x = MAXSIZE/numprocs;
	int *B = malloc(sizeof(int)*x);

	MPI_Scatter(A, x, MPI_INT, B, x, MPI_INT, 0, MPI_COMM_WORLD);
	
	myresult=0;
	for(i=0;i<x;i++) if (B[i]%2==1) myresult++;
	free(B);

	MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0) printf("There are %d odd integers.\n", result);
	MPI_Finalize();

}