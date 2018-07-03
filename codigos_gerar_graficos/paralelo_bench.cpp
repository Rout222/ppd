#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
using namespace std;

int main()
{
	int nMaior;
	int q, centro, perimetro,contador ;

	for (nMaior = 1; nMaior < 360; ++nMaior)
	{
		q = 4;
		int matriz[2*nMaior+1][2*nMaior+1];
		int cortes[q][4];

    // pega todos os cortes que ele deseja fazer
		for (int i = 0; i < q; i++)
		{
			cortes[i][0] = -nMaior;
			cortes[i][1] = -nMaior;
			cortes[i][2] = nMaior;
			cortes[i][3] = nMaior;
		}
    // preenchendo a matriz com os valores da espiral, indo anel por anel
		centro = nMaior;
		matriz[centro][centro] = 1;
		int  x, dx, y, dy, aux;
		clock_t begin_time = clock();

	#pragma omp parallel for private(x, y, dx, dy, contador, aux, perimetro) shared(matriz)
		for (int n = 1; n <= nMaior; n++)
		{
			perimetro = (2*(2*n+1))+(2*((2*n+1)-2));
			dx = 1;
			dy = 0;
			x = n;
			y = -n+1;
			contador = (2*(n-1)+1)*(2*(n-1)+1)+1; 
			for (int i = 0; i < perimetro; ++i)
			{
				matriz[centro+y][centro+x] = contador++;
				if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1-y)){
					aux = dy;
					dy = dx;
					dx = -aux;
				}
				x = x+dx;
				y = y+dy;
			}
		}
	cout <<nMaior<< ", "<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ", parallel" << ", vector" << endl;
		int sum;
		begin_time = clock();
    	#pragma omp parallel for private(sum)
		for (int n = 0; n < q; ++n)
		{
			sum = 0;
			for (int i = min(cortes[n][2],cortes[n][0]); i <= max(cortes[n][2],cortes[n][0]); ++i)
			{
				for (int j = min(cortes[n][1],cortes[n][3]); j <= max(cortes[n][1],cortes[n][3]); ++j)
				{
					sum += matriz[centro+j][centro+i];\
				}
			}
		}
		cout <<nMaior<< ", "<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ", parallel" << ", sum" << endl;
	}
	return 0;
}