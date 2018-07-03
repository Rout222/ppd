#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#include <mpi.h>

using namespace std;

int main()
{
	MPI_Init(NULL, NULL);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	cout<<"rank = "<<rank<<endl;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int nMaior;
	int q, centro, perimetro,contador ;
	clock_t begin_time;
	for (int auxi = 0; auxi < 360; ++auxi)
	{
		if(rank == 0){
			nMaior = auxi;
			q = 4;
		}
		MPI_Bcast(&nMaior, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&q,	   1, MPI_INT, 0, MPI_COMM_WORLD);
		centro = nMaior;

		int cortes[q][4];
		if(rank == 0){
			for (int i = 0; i < q; i++)
			{
				cortes[i][0] = -nMaior;
				cortes[i][1] = -nMaior;
				cortes[i][2] = nMaior;
				cortes[i][3] = nMaior;
			}
		}
	//tentando passar a matriz de cortes
		MPI_Bcast(&cortes, 4*q , MPI_INT, 0, MPI_COMM_WORLD);


		int matriz[2*nMaior+1][2*nMaior+1];
		int matriz_local[2*nMaior+1][2*nMaior+1];
	#pragma omp parallel for
		for (int i = 0; i < 2*nMaior+1; ++i)
		{
			for (int j = 0; j < 2*nMaior+1; ++j)
			{
				matriz[i][j] = 0;
				matriz_local[i][j] = 0;
			}
		}

		matriz[centro][centro] = 1;
		matriz_local[centro][centro] = 1;
    // preenchendo a matriz com os valores da espiral, indo anel por anel
		int  x, dx, y, dy, aux;
		if(rank == 0)
			begin_time = clock();

	#pragma omp parallel for private(x, y, dx, dy, contador, aux, perimetro) shared(matriz)
		for (int n = rank+1; n <= nMaior; n += size)
		{
			perimetro = (2*(2*n+1))+(2*((2*n+1)-2));
			dx = 1;
			dy = 0;
			x = n;
			y = -n+1;
			contador = (2*(n-1)+1)*(2*(n-1)+1)+1; 
			for (int i = 0; i < perimetro; ++i)
			{
				matriz_local[centro+y][centro+x] = contador++;
				if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1-y)){
					aux = dy;
					dy = dx;
					dx = -aux;
				}
				x = x+dx;
				y = y+dy;
			}
		}

	//retorna os valores calculados
		MPI_Reduce(matriz_local, matriz, (2*nMaior+1)*(2*nMaior+1) , MPI_INT, MPI_BOR, 0, MPI_COMM_WORLD);
		if(rank == 0)
			cout <<nMaior<< ", "<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ",mpi" << ",vector" << endl;
		int sum;

		int resultados[q];
		int resultados_local[q];
		for (int i = 0; i < q; ++i)
		{
			resultados[i] = 0;
			resultados_local[i] =0;
		}

		if(rank == 0)
			begin_time = clock();
    #pragma omp parallel for private(sum)
		for (int n = rank; n < q; n+= size)
		{
			sum = 0;

        #pragma omp parallel for reduce(+:sum)
			for (int i = min(cortes[n][2],cortes[n][0]); i <= max(cortes[n][2],cortes[n][0]); ++i)
			{
				for (int j = min(cortes[n][1],cortes[n][3]); j <= max(cortes[n][1],cortes[n][3]); ++j)
				{
					sum += matriz[centro+j][centro+i];
				}
			}
			resultados_local[n] = sum; 
		}
		MPI_Reduce(resultados_local, resultados, q , MPI_INT, MPI_BOR, 0, MPI_COMM_WORLD);
		if(rank == 0)
			cout <<nMaior<< ", "<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ",mpi" << ",sum" << endl;
	}
	return MPI_Finalize();
}