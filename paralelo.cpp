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
    int nMaior;
    int q, centro, perimetro,contador ;
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
    // preenchendo a matriz com os valores da espiral, indo anel por anel
    centro = nMaior;
    matriz[centro][centro] = 1;
    int  x, dx, y, dy, aux;

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

    for (int i = (2*nMaior); i >= 0 ; --i)
    {
        for (int j = 0; j < (2*nMaior+1); ++j)
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