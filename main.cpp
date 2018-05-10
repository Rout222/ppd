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
        
    }
    myfile.close();



    // preenchendo a matriz com os valores da espiral, indo anel por anel

    int centro = n;
    int  x = 0, dx = 0, y = 0, dy = -1, aux;
    //#pragma omp parallel for reduction(+:area)
    for (int i = 0; i < (2*n+1) * (2*n+1); ++i)
    {
        if ((-n/2 < x <= n/2) && (-n/2 < y <= n/2)){
             matriz[y+centro][x+centro] = i+1;
        }
        if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1-y)){
            aux = dy;
            dy = dx;
            dx = -aux;
        }
        x = x+dx;
        y = y+dy;
    }

    for (int i = (2*n); i >= 0 ; --i)
    {
        for (int j = 0; j < (2*n+1); ++j)
        {
           cout<<" |\t"<<matriz[i][j]<<"\t";
        }
        cout<<"|"<<endl;
    }
    /*int sum;
    for (int n = 0; n < q; ++n)
    {
        sum = 0;
        cout << cortes[n][0]<<","<<cortes[n][1]<<"->"<<cortes[n][2]<<","<<cortes[n][3]<<endl;
        for (int i = min(cortes[n][2],cortes[n][0]); i <= max(cortes[n][2],cortes[n][0]); ++i)
        {
            for (int j = min(cortes[n][1],cortes[n][3]); j <= max(cortes[n][1],cortes[n][3]); ++j)
            {
                sum += matriz[centro+j][centro+i];
                cout << matriz[centro+j][centro+i] << endl;
            }
        }
        cout << "Corte "<< n << " deu " << sum << endl; 
    }*/

    return 0;
}