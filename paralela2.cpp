#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>


using namespace std;


void llenar_random(int arreglo[])
{
	srand(time(NULL));// numeros random en los que su "semilla" es variable
	int x=0;
	for(int i=0; i<100000;i++)
	{
	x = rand(); //numeros random 
	arreglo[i] = x;
	x =0;
		}
	}	

int media(int arreglo[], int n){
int sum = 0;
for (int i = 0; i < n; i++){
	sum += arreglo[i];
	    }
     return sum / n;
}
	 
int varianza(int arreglo[], int n){
 int mediaV = media(arreglo, n);
 int sum = 0;
 for (int i = 0; i < n; i++){
    sum += (arreglo[i] - mediaV) * (arreglo[i] - mediaV);
	}
 return sum / (n-1);
	}

int desviacion(int varianza){
 int des = sqrt(varianza);
 return des;
}

int main(){ // esta cosa es pa probar :v
int n =100000;
int m, v, d;
int arreglo[100000];
llenar_random(arreglo);
m = media(arreglo, n);
v= varianza(arreglo,n);
d = desviacion(v);
cout << "media: "<< m<< endl;
cout << "var: "<< v<< endl;
cout << "des: "<< d<< endl;


}
