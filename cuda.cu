#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
using namespace std;


// laerte, caso de erro, execute o arquivo ./codigos_gerar_graficos/cuda_bench.cu

_global_
void d_main(int num_threads, int nMaior, int centro, int* matriz){
	int aux = blockDim.x * blockIdx.x + threadIdx.x;
	int n;
	int  x, dx, y, dy, perimetro, contador;
	for (n = aux+1; n <= nMaior; n += blockDim.x * num_threads)
	{
		perimetro = (2*(2*n+1))+(2*((2*n+1)-2));
		dx = 1;
		dy = 0;
		x = n;
		y = -n+1;
		contador = (2*(n-1)+1)*(2*(n-1)+1)+1; 
		for (int i = 0; i < perimetro; i++)
		{
			matriz[(centro+y)*(2*nMaior+1) + (centro+x)] = contador++;
			if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1-y)){
				aux = dy;
				dy = dx;
				dx = -aux;
			}
			x = x+dx;
			y = y+dy;
		}
	}
}

int main()
{
	int nMaior;
	int q, centro;
	ifstream myfile ("input.txt");
	myfile >> nMaior;
	myfile >> q;
	int matriz[2*nMaior+1][2*nMaior+1];
	int cortes[q][4];

    // pega todos os cortes que ele deseja fazer
	for (int i = 0; i < q; i++)
	{
		for (int j = 0; j < 4; ++j)
		{
			myfile >> cortes[i][j];
		}

	}
	myfile.close();
	q = 4;
	int dim = 2*nMaior+1;
	int* d_matriz;
	cudaMalloc(&d_matriz, dim*dim*sizeof(int));

    // preenchendo a matriz com os valores da espiral, indo anel por anel
	centro = nMaior;
	matriz[centro][centro] = 1;
	cudaMemcpy(d_matriz, matriz, dim*dim*sizeof(int), cudaMemcpyHostToDevice);
	int num_threads = 16;
	int num_blocks  = 32;
	clock_t begin_time = clock();
	d_main<<<num_threads,num_blocks>>>(num_threads, nMaior, centro,d_matriz);
	cout <<nMaior<< ","<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ",cuda" << ",vector" << endl;
	cudaMemcpy(matriz, d_matriz, dim*dim*sizeof(int), cudaMemcpyDeviceToHost);
	int sum;
	begin_time = clock();
	for (int n = 0; n < q; ++n)
	{
		sum = 0;
		for (int i = min(cortes[n][2],cortes[n][0]); i <= max(cortes[n][2],cortes[n][0]); ++i)
		{
			for (int j = min(cortes[n][1],cortes[n][3]); j <= max(cortes[n][1],cortes[n][3]); ++j)
			{
				sum += matriz[centro+j][centro+i];
			}
		}
	}
	cout <<nMaior<< ", "<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ",cuda" << ",sum" << endl;
	cudaFree(&d_matriz);
	
	return 0;
}