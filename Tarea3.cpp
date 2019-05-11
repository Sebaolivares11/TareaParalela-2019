#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//cabecera de las llamadas a MPI
#include <mpi.h>
#include <time.h>
#include <tgmath.h>

#define N 10 //tamaño arreglo


// ---------------------------------------------FUNCIONES :<----------------------------------------------------------------//
void llenar_random(int arreglo[], int tam) // se le pasa el arreglo como parametro  y se define un tamaño
{
	srand(time(NULL));// numeros random en los que su "semilla" es variable
	int x=0;
	for(int i=0; i<tam;i++)
	{
	x = rand() %10000+1; //numeros random
	arreglo[i] = x;
		}
	}

float var (int arreglo[], float prom)// funcion que calcula la varianza de un arreglo
{
	float varianza;
	for(int i=0; i<N; i++)
	{
		varianza += pow(arreglo[i]-prom, 2);
		varianza = varianza/prom;
	}
	return varianza;
}

float des_estandar(float v)
{
	float des = sqrt(v);
	return des;
}

int* crear_sub_vec(int begin, int end, int* origin){ // crea un sub vector para enviar a un proceso en paralelo
    int* sub_vec;
    int size;
    int aux=0;
    size = end - begin;
    sub_vec= (int*)malloc(sizeof(int));
    for(int i= begin; i<end; i++){
      *(sub_vec+aux)= *(origin+i);
      aux +=1;
    }
    return sub_vec;
  }

  void print(int my_rank, int comm_sz, int n_over_p, int* sub_vec){
    printf("Proceso %d de %d recibe el  sub vector: [ ", my_rank, comm_sz);
    for(int i=0; i< n_over_p; i++){
      printf("%d,",*(sub_vec+i));
    }
    printf("]\n");
  }

  float promedio_sub_vector(int* sub_vec, int tam_sub_vec){
    float promedio;
    float subsuma;
    int *aux=sub_vec;
    for(int i=0; i<tam_sub_vec;i++)
    {
      subsuma += *aux;
      aux++;
    }
    promedio = subsuma/tam_sub_vec;
    return promedio;
  }

int* crear_sub_vec(int begin, int end, int* origin);
void print(int my_rank, int comm_size, int n_over_p, int* sub_vec);
void llenar_random(int arreglo[], int tam);



////////////////////////////////////////--------------Funciones END---------//////////////////////////////////////////////////////////////




int main(void) {
    int comm_sz; //son los procesos
    int my_rank; // procesasor
    int n_over_p; //cantidad de datos  dividido en cantidad de procesos
    int arreglo[N]; // se inicia el arreglo
    int* sub_vec = NULL;// sub vector inicializado en null para pasarle datos
		float s_promedio; // suma de los promedio con el fin de calcular el promedio total
		float sub_promedio_i;
		float sub_promedio_0; // variable que entregara cada promedio
		float p_total; // resultado de los promedios
		float desviacion_estandar; // resultado de la desviacion.
		float varianza_total;
    llenar_random(arreglo, N);
    MPI_Status estado; /* devuelve estado al recibir*/
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    n_over_p = (N/comm_sz);
		printf("Distribuyendo datos\n" );
    printf("proceso %d Calcula un arreglo de %d  datos\n", my_rank, n_over_p);

    if(my_rank != 0){ // procesos paralelos distintos de 0
      sub_vec = (int*)malloc(n_over_p * sizeof(int));
      MPI_Recv(sub_vec, n_over_p, MPI_INT,0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      print(my_rank, comm_sz ,n_over_p,sub_vec);
      sub_promedio_i = promedio_sub_vector(sub_vec, n_over_p);
      printf("Subpromedio: %lf\n",sub_promedio_i );
      MPI_Send(&sub_promedio_i,1,MPI_FLOAT,0,0,MPI_COMM_WORLD);// envia los subpromedios al proceso 0
    }


    else { //esto es lo que hace el proceso cero
      printf("Distribuyendo datos\n" );

      for(int i=1; i<comm_sz;i++){
        sub_vec=crear_sub_vec(i*n_over_p,(i*n_over_p)+n_over_p,arreglo);
        MPI_Send(sub_vec, n_over_p, MPI_INT, i,0, MPI_COMM_WORLD);

      }

      sub_vec= crear_sub_vec(0,n_over_p,arreglo);
      print(my_rank, comm_sz ,n_over_p,sub_vec);
      sub_promedio_0= promedio_sub_vector(sub_vec,n_over_p);
      printf("Subpromedio: %lf\n", sub_promedio_0 );
       // condicion para que se ejecuten los calculos al terminar los procesos paralelos
      for(int j=1 ;j<comm_sz; j++)
        {
          MPI_Recv(&sub_promedio_i,1,MPI_FLOAT,j,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
					s_promedio += sub_promedio_i;
        }
			printf("*********Datos Finales del proceso********** \n" );
			s_promedio+= sub_promedio_0;
			p_total = s_promedio/comm_sz; // calcula el promedio total final
			varianza_total = var(arreglo,p_total);
			desviacion_estandar=des_estandar(varianza_total);
			printf("Promedio general %lf\n", p_total);
			printf("Varianza general %lf\n", varianza_total);
			printf("Desviacion estandar general %lf\n", desviacion_estandar);
      }





    MPI_Finalize();
    return 0;
}
