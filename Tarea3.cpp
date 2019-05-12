#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* Cabecera llamadas MPI */
#include <mpi.h>

#define N 100000 //tamaño arreglo fixeado con sus respectivas pruebas


// ---------------------------------------------FUNCIONES -----------------------------------------------------------------//

void llenar_random(int arreglo[], int tam) // se le pasa el arreglo como parametro  y se define un tama�o
{
	int x;
	srand(time(NULL));// numeros random en los que su "semilla" es variable
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

//-----------------------

float estandar(float v)
{
	float des = sqrt(v);
	return des;
}

//--------------------------

int* crear_sub_vec(int begin, int end, int* origin){ // crea un sub vector para enviar a un proceso en paralelo
    int* sub_vec;
    int size;
    int aux=0;
    size = end - begin;
    sub_vec= (int*)malloc(size*sizeof(int)); //
    for(int i= begin; i<end; i++){
      *(sub_vec+aux)= *(origin+i);
      aux +=1;
    }
    return sub_vec;
  }
/*
  void print(int my_rank, int comm_sz, int n_over_p, int* sub_vec){
    printf("Proceso %d de %d recibe el  sub vector: [ ", my_rank, comm_sz);
    for(int i=0; i< n_over_p; i++){
      printf("%d,",*(sub_vec+i));
    }
    printf("]\n");
  }
*/
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





////////////////////////////////////////--------------Funciones END---------//////////////////////////////////////////////////////////////




int main(void) {

	//USOS PARA MPI
    int comm_sz; //son los procesos
    int my_rank; // procesador
    int n_over_p; //cantidad de datos  dividido en cantidad de procesos
    int arreglo[N]; // se inicia el arreglo
    int* sub_vec = NULL;// sub vector inicializado en null para pasarle datos
		float p_total;
		float sub_promedio_0, sub_promedio_i;
		float desviacion_estandar; // resultado de la desviacion.
		float varianza_total;
		//Se llena el arreglo random
    llenar_random(arreglo, N);

		//inicio MPI
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Status estado;

		
		n_over_p = (N/comm_sz);
    if(my_rank != 0){ // procesos paralelos distintos de 0
      sub_vec = (int*)malloc(n_over_p * sizeof(int));
      MPI_Recv(sub_vec, n_over_p, MPI_INT,0,0, MPI_COMM_WORLD, &estado);
    	/*print(my_rank, comm_sz ,n_over_p,sub_vec); // funcion que mostraba los datos del arreglo para comprobar*/
      float sub_promedio_i = promedio_sub_vector(sub_vec, n_over_p);
      printf("Subpromedio: %lf\n",sub_promedio_i );
      MPI_Send(&sub_promedio_i,1,MPI_FLOAT,0,0,MPI_COMM_WORLD);// envia los subpromedios al proceso 0
    }


    else { //esto es lo que hace el proceso cero
      printf("Distribuyendo datos\n" );

      for(int i=1; i<comm_sz;i++){
        	sub_vec = crear_sub_vec(i*n_over_p,(i*n_over_p)+n_over_p,arreglo);
        	MPI_Send(sub_vec, n_over_p, MPI_INT, i,0, MPI_COMM_WORLD);
      }

			sub_vec = crear_sub_vec(0,n_over_p,arreglo);
    /*  print(my_rank, comm_sz ,n_over_p,sub_vec); //Funcion que imprime el sub arreglo*/
      float sub_promedio_0 = promedio_sub_vector(sub_vec,n_over_p);
      printf("Subpromedio: %lf\n", sub_promedio_0 );

      for(int i=1 ;i<comm_sz; i++) // para cada proceso se juntan en el 0
        {
					sub_promedio_i= sub_promedio_i;
          MPI_Recv(&sub_promedio_i,1,MPI_FLOAT,i,0,MPI_COMM_WORLD,&estado);
					sub_promedio_0 += sub_promedio_i;
        }
			printf("*********Datos Finales del proceso********** \n" );
			p_total = (sub_promedio_0/comm_sz); // calcula el promedio total final
			printf("Promedio general %lf\n", p_total);
			printf("Varianza general %lf\n", var(arreglo,p_total));
			//float desviacion = (sqrt(var(arreglo,p_total)));
			printf("Desviacion estandar general %lf\n", estandar(var(arreglo,p_total)));
      }

    MPI_Finalize();
    return 0;
}
