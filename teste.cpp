#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

int main()
{
	int nMaior = 5;
	for (int n = 1; n <= nMaior; ++n)
	{
	int perimetro = (2*(2*n+1))+(2*((2*n+1)-2));
	int dx = 1, dy = 0, aux;
	int x = n, y = -n+1;

	int foo = (2*(n-1)+1)*(2*(n-1)+1)+1; 
	for (int i = 0; i < perimetro; ++i)
    {
    	cout << x << " | " << y << " = " << foo++ << endl;
        if ((-n/2 < x <= n/2) && (-n/2 < y <= n/2)){
            // matriz[y+centro][x+centro] = i+1;
            // cout << (x) << " | " << (y) << endl;
        }
        if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1-y)){
            aux = dy;
            dy = dx;
            dx = -aux;
        }
        x = x+dx;
        y = y+dy;
    }
    cout << endl;
	}
}

