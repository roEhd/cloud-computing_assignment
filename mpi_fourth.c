#include <mpi.h>
#include <stdio.h>

void main(int argc, char *argv) {
	int myid, numprocs;
	int  i, j;
	
	MPI_Datatype columntype;
	int A[5][5]={0};
	int B[5][5]={0};

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if(myid == 0) for(i=0;i<5;i++) for(j=0;j<5;j++) A[i][j] = i*5 + j;
	else if(myid == 1) for(i=0;i<5;i++) for(j=0;j<5;j++) B[i][j] = i*5 + j + 50;

	MPI_Type_vector(5, 1, 5, MPI_INT, &columntype);
	MPI_Type_commit(&columntype);
	
	if(myid == 0) MPI_Bcast(&A[0][3], 1, columntype, 0, MPI_COMM_WORLD);
	if(myid == 1) MPI_Bcast(&B[0][4], 1, columntype, 0, MPI_COMM_WORLD);

	if(myid == 1) MPI_Bcast(&B[0][2], 1, columntype, 1, MPI_COMM_WORLD);
	if(myid == 0) MPI_Bcast(&A[0][4], 1, columntype, 1, MPI_COMM_WORLD);

	if(myid == 1)
	{
		for(i=0;i<5;i++)
		{	
			for(j=0;j<5;j++)	
			printf("%5d", B[i][j]);		
			printf("\n");
		}
	}
	MPI_Finalize();

}