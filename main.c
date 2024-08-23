#include <stdio.h>
#include <math.h> // este header es importante para poder usar pow 

float calcula_imc(float,float); //prototipo de la función, declarada la función, lleva punto y coma

int main(int argc, char const *argv[])

{
    float estatura, peso, imc;
    char Nombre[30];

    printf("Ingrese su nombre\n");
    fgets(Nombre,30,stdin);

    printf("ingrese su peso \n");
    scanf("%f",&peso);

    printf("Ingrese estatura\n");
    scanf("%f",&estatura);

    imc = calcula_imc(peso, estatura);


    //para imprimir el resultado:
    printf("%s su IMC = %f\n",Nombre,imc);

    printf("Sistemas computacionales ♥\n");

    return 0;
}

float calcula_imc(float peso, float estatura) //no lleva punto y coma, se hace luego de que esté declarada la función en la parte de arriba
{
    float IMC; 
    //IMC =peso/estatura al cuadrado 

    IMC = peso / pow(estatura,2);
    return IMC; 


}