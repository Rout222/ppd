#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
using namespace std;

int main(int argc, char const *argv[])
{
    int n, q;

    for (n = 1; n < 360; n++) // de tamanho 1 ao tamanho 750
    {
        int matriz[2*n+1][2*n+1];
        int cortes[4][4];
        q = 4; 
        for (int i = 0; i < q; i++) // sempre executar 4 cortes, de tamanho maximo
        {
            cortes[i][0] = -n; 
            cortes[i][1] = -n; 
            cortes[i][2] = n; 
            cortes[i][3] = n; 
        }

        int centro = n;
        int  x = 0, dx = 0, y = 0, dy = -1, aux;

        clock_t begin_time = clock();

        for (int i = 0; i < (2*n+1) * (2*n+1); ++i)
        {
            if ((-n/2 < x <= n/2) && (-n/2 < y <= n/2))
            {
                matriz[y+centro][x+centro] = i+1;
            }
            if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1-y))
            {
                aux = dy;
                dy = dx;
                dx = -aux;
            }
            x = x+dx;
            y = y+dy;
        }
        cout <<n<< ", "<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ", serial" << ", vector" << endl;
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
        cout <<n<< ", "<<  float( clock () - begin_time ) /  CLOCKS_PER_SEC << ", serial" << ", sum" << endl;
    }
    
    return 0;
}