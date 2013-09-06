/**
    Alejandro Duarte Sanchez
    Sistemas Operativos

**/

/*********************************************************************************************************
    PENDIENTES: NONE

**********************************************************************************************************/
//practica Actual: 8

#include <stdio.h>
#include <stdlib.h>

#include "operaciones.h"


int main()
{
    unsigned int num;
    struct proceso p[MAX];
    struct proceso t[MAX];

    printf("\n\n\tIntroduzca el numero de procesos: ");
    scanf("%i",&num);

    insertar(p,num);

    mostrar(p,t,&num);

    system("cls");
    draw_timeTable(t,num);

    return EXIT_SUCCESS;

}
