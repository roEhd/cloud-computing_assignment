# cloudcomputing_project

## mpi_first.c
프로세스 P0 는 정수 배열 A[1000]를 임의의 양수로 채워 놓는다. 이 배열에 홀수가 몇 개 들어있는지 계산하는 효율적인 MPI 프로그램을 작성하시오. 이 프로그램을 실행할 때는 프로세스 개수를 2개, 4개, 5개, 10개로만 테스트해 볼 예정이므로, 배열은 언제나 균등하게 프로세스들에게 배분될 수 있다고 가정해도 된다.

## mpi_second.c
다음과 같이 진행되는 MPI 프로그램을 작성하시오.  
1 프로그램에 2차원 정수 배열 A[10][10]을 선언한다.  
2 프로세스 0은 이 배열의 각 원소를 다음과 같은 값으로 채운다. (A[i][j] = i * 10 + j).  
3 프로세스 0은 MPI_Bcast( )를 통해 다른 프로세스들에게 배열 A[ ][ ]의 값을 복사해 준다.  
4 프로세스 1은 (대표로) 자신의 배열 A[ ][ ]를 화면에 출력한다.  

## mpi_third.c
이 프로그램은 프로세스 5개로만 실행할예정이라고 가정하시오.  
1 프로그램에 2차원 정수 배열 A[10][10]과 B[2][10]을 선언한다.  
2 프로세스 0은 배열 A의 각 원소를 다음과 같은 값으로 채운다. (A[i][j] = i * 10 + j).  
3 프로세스 0은 MPI_Scatter( )를 통해 배열 A[ ][ ]의 값을 프로세스마다 2행씩 나누어 준다. 전송받게 되는 2행은 정수 배열 B[2][10]에 저장한다.  
4 프로세스3은 (대표로) 자신의 배열 B[ ][ ]를 화면에 출력한다.  

## mpi_fourth.c
이 프로그램은 프로세스 2개로 실행한다고 가정한다. (힌트: MPI_Type_vector 활용)  
1 프로그램에 2차원 정수 배열 A[5][5]과 B[5][5]를 선언하고 모두 0으로 초기화해 놓는다.  
2 프로세스 0은 배열 A를 다음과 같은 값으로 채운다. (A[i][j] = i * 5 + j).  
3 프로스세 1은 배열 B를 다음과 같은 값으로 채운다. (B[i][j] = i * 5 + j + 50).  
4 프로세스 0은 배열 A의 3번 열을 프로세스 1에게 보낸다. 프로세스 1은 받은 열을 배열 B의 4번 열에 저장한다.  
5 프로세스 1은 배열 B의 2번 열을 프로세스 0에게 보낸다. 프로세스 0은 받은 열을 배열 A의 4번 열에 저장한다.  
6 프로세스 1은 배열 B를 화면에 출력한다.  

## mpi_fifth.c
다음은 벽난로 문제를 푸는 순차 프로그램이다. 이 프로그램과 동일한 일을 하는 MPI 프로그램을 작성하시오. MPI 프로그램은 프로세스 4개 또는 5개를 사용하여 실행할 때 문제없이 실행되어야 한다. 각 프로세스가 담당할 구역은 임의로 정할 수 있으나, 프로세스당 workload는균등해야 한다. 프로그램은 다음 사항을 가정한다.  
1 방의 크기는 102x102이다. 가장자리 외벽을 제외한 내부의 크기는 100x100이다.  
2 방의 외벽과 내부의 초기 온도는 0도이다. 외벽의 온도는 항상 0도이다.  
3 벽난로는 방의 북쪽에 있으며, 항상 200도의 열을 낸다. (위치 R[0][40]~R[0][59] 지점)  
4 방의 내부 (100x100)의 온도만 10,000 회 update 한다. (벽난로와 외벽의 온도는 고정)  
5 방의 온도를 update 할 때는 배열 2개를 사용하지 않고, 원래 배열 상에서 그대로(프로그램에서와 같이) update 한다.  
6 한참 후의 방의 온도 계산이 끝나고 나면, 사용자가 지정하는 구역의 온도를 화면에 출력해 준다.  
