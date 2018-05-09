#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;


void imprime(int * m , int n){
    for (int i = 0; i < 2*n+1; ++i)
    {
        for (int j = 0; j < 2*n+1; ++j)
        {
            cout<<m[i*n+j]<<" ";
        }
        cout<<endl;
    }
}

int main()
{
    ifstream myfile ("input.txt");
    int n, q;
    myfile >> n;
    myfile >> q;
    int matriz[2*n+1][2*n+1];
    int cortes[q][4];

    // pega todos os cortes que ele deseja fazer
    for (int i = 0; i < q; i++)
    {
        for (int j = 0; j < 4; ++j)
        {
            myfile >> cortes[i][j];
        }
        //cout << cortes[i][0] << cortes[i][1] << cortes[i][2] << cortes[i][3] << endl;
    }
    myfile.close();



    // preenchendo a matriz com os valores da espiral, indo anel por anel

    int centro = n;
    int  x = 0, dx = 0, y = 0, dy = -1, aux;
    for (int i = 0; i < (2*n+1) * (2*n+1); ++i)
    {
        if ((-n/2 < x <= n/2) && (-n/2 < y <= n/2)){

            matriz[x+centro][y+centro] = i;
        }
        if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1-y)){
            aux = dy;
            dy = dx;
            dx = -aux;
        }
        x = x+dx;
        y = y+dy;
    }
    for (int i = 0; i < (2*n+1); ++i)
    {
        for (int j = 0; j < (2*n+1); ++j)
        {
           cout<<matriz[i][j]<<" ";
        }
        cout<<endl;
    }
    //imprime((int *) matriz,n);
    return 0;
}