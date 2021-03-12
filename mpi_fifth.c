#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv){

	int myid, numprocs;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	double R[102][102] = {0.0}; //방안 초기 온도  
	int i, j, m1, m2, n1, n2;
	for( j=40; j<60; j++) R[0][j]=200.0; //벽난로의 온도(계속 유지)
		
	int row = 100/numprocs; //(가로로 잘랐을 때)각 프로세스가 맡아서 계산할 행의 개수		
	int prev = (myid-1+numprocs)%numprocs; //프로세스를 순서대로 나열했을때 전 순서의 프로세스
	int next = (myid +1)%numprocs; //프로세스를 순서대로 나열했을 때 후 순서의 프로세스

	int k=0; //각자 프로세스 실행 횟수 count
	while(k!=10001){ //1만회 실행한다

		/* 새로운 2차원 배열에 저장하고 한 번에 업데이트 하는 것이 아니라, 각 원소마다(R[i][j]) 계산 후 바로 갱신하고 그 다음 원소 계산에 갱신된 값이 반영(R[i][j-1], R[i-1][j])이 된다.*/
		/* 
		   1. 각 프로세스는 앞 순서의 프로세스(이번 루프)와 뒤 순서의 프로세스(바로전 루프)의 정보가 동기화 되었을 때(receive) 계산을 실행한다.
		   2. 계산을 하고 나면 앞 순서와 뒤 순서에게 동기화를 시켜준다. (send)
		   3. 본인 프로세스의 루프 실행 횟수 count ++
		   동기화의 태그는  send시 자신의 id를 붙여서 보내고 receive시 정보가 뒤섞이지 않도록 prev와 next로 태그를 확인한다.
		*/

		if (myid == 0){
			/*첫번째 프로세스는 혼자 계산하고 Send로 루프를 initiate 한다. 그 이후부터 가장자리를 동기화하고 같은 과정을 반복한다.*/
			if(k!=0) MPI_Recv( &R[row*(myid+1)+1][0], 102, MPI_DOUBLE, next, next, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //두번째 루프부터 뒤 프로세스가 전 단계 계산을 끝냈는지 확인 (동기화)
				for(i=1+row*myid;i<=row*(myid+1);i++) 
					for(j=1;j<=100;j++) R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]); //계산 실행
				k ++; 
			MPI_Send( &R[row*(myid+1)][0],  102, MPI_DOUBLE, next, myid, MPI_COMM_WORLD); //뒤에게 계산이 끝났음을 알림 (동기화)


		}
		else if (myid != numprocs-1){
			/*중간 프로세스는 initiate를 기다렸다가 루프를 실행한다.*/
			MPI_Recv( &R[row*myid][0],  102, MPI_DOUBLE, prev, prev ,MPI_COMM_WORLD, MPI_STATUS_IGNORE); //앞한테 바통 받음 (동기화)
			if(k!=0) MPI_Recv( &R[row*(myid+1)+1][0], 102, MPI_DOUBLE, next, next ,MPI_COMM_WORLD, MPI_STATUS_IGNORE); //두번째 루프부터 뒤 프로세스가 전 단계 계산을 끝냈는지 확인 (동기화)
				for(i=1+row*myid;i<=row*(myid+1);i++) 
					for(j=1;j<=100;j++) R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				k ++;
			MPI_Send( &R[1+row*myid][0], 102, MPI_DOUBLE, prev, myid, MPI_COMM_WORLD); //앞한테 해도 좋다고 알림 (동기화)
			MPI_Send( &R[row*(myid+1)][0], 102, MPI_DOUBLE, next, myid, MPI_COMM_WORLD); //뒤에게 계산이 끝났음을 알림 (동기화)
		}
		else {
			/*맨 끝 프로세스는 동기화를 받아서 계산한 뒤, 다시 전 프로세스에게 보내주는 과정만 반복한다.*/
			MPI_Recv( &R[row*myid][0], 102, MPI_DOUBLE, prev, prev,MPI_COMM_WORLD, MPI_STATUS_IGNORE); //앞한테 바통 받음 (동기화)
				for(i=1+row*myid;i<=row*(myid+1);i++)
					for(j=1;j<=100;j++) R[i][j]=0.25*(R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
				k ++;
			MPI_Send( &R[1+row*myid][0],  102, MPI_DOUBLE, prev, myid, MPI_COMM_WORLD); //앞한테 해도 좋다고 알림 (동기화)

		}
		
	}
		
	MPI_Barrier(MPI_COMM_WORLD); //모든 프로세스가 loop를 끝낼때 까지 기다림
	
	if(myid !=0) MPI_Send(&R[1+row*myid][0], 102*row, MPI_DOUBLE, 0, 999, MPI_COMM_WORLD); //loop를 끝낸 프로세스는 계산한 정보를 (동기화하는 가장자리 제외) 0에게 보냄
		
	if (myid ==0)for(i=1;i<numprocs;i++) MPI_Recv(&R[1+row*i][0], 102*row, MPI_DOUBLE, i, 999, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //0에서 순서대로 정보를 받아옴	

	if(myid == 0){
		//결과 테스트를 위한 구간 출력
		printf("출력할 구간 (m1, m2, n1, n2) : ");
		scanf("%d %d %d %d", &m1, &m2, &n1, &n2);
		
		for(i=m1; i<=m2; i++){ //방의 세로 구간
			for(j=n1; j<=n2; j++) //방의 가로 구간
				printf("%7.1f", R[i][j]);
			printf("\n");
			}
	
	}			

	MPI_Finalize();
	
}