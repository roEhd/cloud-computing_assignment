#include <mpi.h>
#include <stdio.h>

void main(int argc, char *argv) {
	int myid, numprocs;
	int  i, j;
	
	int A[10][10]={0};

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if(myid == 0)
	{
		for(i=0;i<10;i++) for(j=0;j<10;j++) A[i][j] = i*10 + j;
		MPI_Bcast(A, 100, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else	
		MPI_Bcast(A, 100, MPI_INT, 0, MPI_COMM_WORLD);
	if(myid == 1)
	{
		for(i=0;i<10;i++)
		{	
			for(j=0;j<10;j++)	
			printf("%5d", A[i][j]);		
			printf("\n");
		}
	}
	MPI_Finalize();

}