#include "minimal.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
    //Pedir al usuario que ingrese dos numeros y calcule el menor de ellos.
    int numero1, numero2, potencia ;

    printf("Ingrese primer número\n");
    scanf("%i",&numero1);

    printf("Ingrese segundo número\n");
    scanf("%i",&numero2);

    printf("the min value is: %d\n",minimal(numero1,numero2));
   
    //usar la funcion pow() para elevar el primer número al segundo
    potencia=pow(numero1,numero2);

     printf("%i elevado a la %i es = %i",numero1, numero2, potencia);
     
    return 0;

}