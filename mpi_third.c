#include <mpi.h>
#include <stdio.h>

void main(int argc, char *argv) {
	int myid, numprocs;
	int  i, j;
	
	int A[10][10]={0};
	int B[2][10]={0};

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if(myid == 0) for(i=0;i<10;i++) for(j=0;j<10;j++) A[i][j] = i*10 + j;
	
	MPI_Scatter(A, 20, MPI_INT, B, 20, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(myid == 3)
	{
		for(i=0;i<2;i++)
		{	
			for(j=0;j<10;j++)	
			printf("%5d", B[i][j]);		
			printf("\n");
		}
	}
	MPI_Finalize();

}