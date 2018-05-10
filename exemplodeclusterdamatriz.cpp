#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<omp.h>

int main(int argc, char **argv){
	int linhas_a, linhas_b, colunas_a, colunas_b;
	int *a, *b, *c;
	int linha,coluna,k,*resultado;
	FILE *fa,*fb,*fc;
	int rank, size;

	int i;
	printf("OK\n");
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_rank(MPI_COMM_WORLD,&size);

	printf("OK\n");

	if(rank == 0) {
		fa = fopen("matriza", "r");
		fb = fopen("matrizb", "r");

		//
		// Ler a matriz A
		//
		fscanf(fa,"%d %d", &linhas_a, &colunas_a);
		a = (int*)calloc(linhas_a*colunas_a, sizeof(int));


		for(i=0;i<linhas_a*colunas_a;i++){
			fscanf(fa,"%d", &a[i]);
		}
		

	//
	//Ler a matriz B
	//

	fscanf(fa,"%d %d", &linhas_b, &colunas_b);
		b = (int*)calloc(linhas_b*colunas_b, sizeof(int));


		for(i=0;i<linhas_a*colunas_a;i++){
			fscanf(fb,"%d", &b[i]);
		}

		fclose(fa);
		fclose(fb);
	}

	MPI_Bcast(&linhas_a,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&colunas_a,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&linhas_b,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&colunas_b,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank != 0){
		a = (int*)calloc(linhas_a*colunas_a,sizeof(int));
		b = (int*)calloc(linhas_b*colunas_b,sizeof(int));
	}

	MPI_Bcast(a,linhas_a*colunas_a, MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(b,linhas_b*colunas_b, MPI_INT,0,MPI_COMM_WORLD);
	c = (int*)calloc(linhas_a*colunas_b,sizeof(int));



	// Inicia o meu vetor
	#pragma omp parallel for
	for(i=0;i<linhas_a*colunas_b;i++) {
		c[i] = 0;
	}

	#pragma omp parallel for private(coluna,linha,k)
	for(i=rank;i<linhas_a*colunas_b;i+=size) {
		coluna = i%colunas_b;
		linha = (i-coluna)/colunas_b;

		for(k=0;k<colunas_a;k++) {
			c[i] = a[linha*colunas_a+k]*b[k*colunas_b+coluna];
		}
	}

	if(rank == 0){
		resultado = (int*)calloc(linhas_a*colunas_b, sizeof(int));
	}
	MPI_Reduce(resultado, c, linhas_a*colunas_b ,MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);

	if(rank == 0) {
		fc = fopen("matrizc","w");


		fprintf(fc,"%d %d\n",linhas_a,colunas_b);

		for(i=0;i<linhas_a*colunas_b;i++) {
			printf("%d\n",resultado[i]);
			fprintf(fc,"%d",resultado[i]);

			if(i%colunas_b == 0 && i != 0) {
				fprintf(fc,"\n");
			}else{
				fprintf(fc," ");
			}
		}

		fclose(fc);
	}


	free(a);
	free(b);
	free(c);
	free(resultado);

	MPI_Finalize();

	return 0;
}