/*
 *operaciones.h
 *Functions for creating an OS
 *created by Alejandro Duarte Sanchez
 *version 10/12/2012
*/
#ifndef OPERACIONES_H_INCLUDED
#define OPERACIONES_H_INCLUDED

#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>
#include "design.h"

#define NUEVO      0
#define LISTO      1
#define EJECUCION  2
#define TERMINADO  3
#define INVALIDO   4
#define BLOQUEADO  5
#define SUSPENDIDO 6

#define FALSE 0
#define TRUE 1
#define MAX 100
#define VEL 400 //800
#define NUM_MARCOS 25


typedef char String[100];

struct tiempo
{
    unsigned int llegada;
    unsigned int finalizacion;
    unsigned int retorno;
    int respuesta;
    unsigned int espera;
    unsigned int servicio;

};
struct marco
{
    unsigned int num;
    unsigned int pos[25];
};
struct proceso
{
    String operacion;
    double resultado;
    unsigned int tiempo_max;
    unsigned int tiempo_trans;
    unsigned int tiempo_restante;
    unsigned int tiempo_bloqueado;
    unsigned int tam;
    struct marco m;
    int id;
    unsigned int estado;//nuevo->0,listos->1,ejecucion->2terminado->3,error->4,bloqueado->5
    struct tiempo t;
};
struct listos
{
    struct proceso proc[20];
};

typedef struct marco Disponibles;

void insertar(struct proceso p[],unsigned int num);
void mostrar(struct proceso p[],struct proceso t[],unsigned int *num);
void creaOperacion(String operacion,double *resultado);
void creaTiempo(unsigned int *tiempo);
void creaTam( unsigned *tam );
int inicializarListos(struct proceso p[],struct listos *l,unsigned int *contListos,unsigned int num , Disponibles *d);
void actualizarListos(unsigned int contListos,struct listos l,int x,int y);
void draw_timeTable(struct proceso ter[],unsigned int num);
void imprimirMarcos(struct listos l , unsigned int contListos , struct listos b, unsigned int contBloqueados);
void imprimirEjecucion(struct listos l,unsigned int ejecutado,unsigned int x2,unsigned int y2);
void mostrarBCP(struct proceso p[],unsigned int cont,struct listos l,unsigned int contListos,struct proceso t[],unsigned int contTerminados, unsigned int num,unsigned int tiempo,struct listos b,unsigned int contBloqueados,struct listos s,unsigned int contSuspendido);
void mostrarListas(unsigned int num,unsigned int cont,unsigned int x,unsigned int y,unsigned int x2,unsigned int x3,unsigned int y2,unsigned int y3,struct proceso t[],unsigned int contTerminados,struct listos l,unsigned int ejecutado,unsigned int bandbloqueo, unsigned int contListos,struct proceso p[],struct listos b, unsigned int contBloqueados,unsigned int tiempo, struct listos s, unsigned int contSuspendido,unsigned int contEspera);
void mostrarTablaPaginas(struct proceso p[] ,unsigned int num);

int kbhit(void);
int getch(void);

void insertar(struct proceso p[],unsigned int num)
{
    unsigned int i;

    //inicializar random
    srand(time(NULL));
    srand (getpid());

    for( i=0; i<num ;i++)
    {
        p[i].id=i;//id automatico
        creaOperacion(p[i].operacion,&p[i].resultado);
        creaTiempo(&p[i].tiempo_max);
        creaTam(&p[i].tam);
        //p[i].tam=30;
        //printf("\ntam[%i]: %i",i,p[i].tam);
        //marcos
        p[i].m.num=p[i].tam/5;
        if( p[i].tam%5 != 0 )
        {
            p[i].m.num+=1;
        }
        //printf("\n\np[%i].marco = %i",i,p[i].m.num);

        p[i].tiempo_restante=p[i].tiempo_max;
        p[i].tiempo_trans=0;
        p[i].t.respuesta=-1;
        p[i].estado=NUEVO;//estado nuevo
    }
        //system("pause");

}
void mostrar(struct proceso p[],struct proceso t[],unsigned int *num)
{
    unsigned  int i,j,k,x,y,x2,yb,y2,x3,y3,bandInt=0,bandbloqueo=0,quantum,marcos_rest;
    unsigned int contTerminados=0,tiempo=0,restante=0,contListos=0,contBloqueados=0,contSuspendido=0,contEspera=0,ejecutado=0,cont=0,bandSusp,timeSusp;
    struct listos l,b,s;
    struct proceso aux;
    Disponibles d;
    char tecla;

    marcos_rest=inicializarListos(p,&l,&contListos,*num, &d);
    cont=contListos;

    ///QUANTUM
    printf("\n\nInserte el tiempo del Quantum: ");
    scanf("%i",&quantum);

    while(contListos>0 || contBloqueados!=0 || contSuspendido > 0 )
    {

        system("cls");
        imprimirMarcos(l,contListos,b,contBloqueados);
        gotoxy(10,1);
        printf("NUEVOS: %i  -->TAM: %i\t\tSUSPENDIDOS: %i  -->TAM: %i\t\tESPERA: %i\n",*num-cont,p[cont].tam,contSuspendido,s.proc[0].tam,contEspera);
        printf("\n\n\tListos\t\t  Proceso en Ejecucion\t\t\tTerminados\n");
        for(i=0;i<95;i++)
            printf("_");

        //se inicializan las coordenadas
        x=0;
        y=6;
        x2=32;
        y2=6;
        x3=60;
        y3=6;

        //se dibuja la tabla
        draw(x,x2,y3);

        //se muestra procesos terminados
        gotoxy(x3-3,y3);
        printf("#Programa     |\tOperacion");
        y3++;
        for(i=0;i<contTerminados;i++)
        {
            gotoxy(x3,y3);
            printf("%i\t\t %s  =",t[i].id,t[i].operacion);
            if(t[i].estado!=4)//estado error
            {
                printf("  %lf  ",t[i].resultado);
            }

            else
                printf("  ERROR");

            y3++;
            draw(x,x2,y3);
        }
        //se muestra procesos listos
        gotoxy(x,y);
        printf("#Programa| TME  | TRE");
        if(bandbloqueo!=1)
        {
            y++;
            actualizarListos(contListos,l,x,y);
        }

        //se muestra proceso en ejecucion
        imprimirEjecucion(l,ejecutado,x2,y2);

        restante=l.proc[ejecutado].tiempo_restante;//variable aux para guardar el tiempo restante

        //opciones de teclas
        gotoxy(30,18);
        printf("- TABLA PAGINAS:  T");
        gotoxy(30,20);
        printf("- INTERRUPCION:   I");
        gotoxy(30,22);
        printf("- ERROR:          E");
        gotoxy(30,24);
        printf("- PAUSA:          A");
        gotoxy(30,26);
        printf("- CONTINUAR:      O");
        gotoxy(30,28);
        printf("- PROCESO NUEVO:  N");
        gotoxy(30,30);
        printf("- BCP:            B");
        gotoxy(30,32);
        printf("- SUSPENDIDO:     S");
        gotoxy(30,34);
        printf("- REGRESA:        R");
        tecla=' ';//se inicializa la tecla para interrupcion

        //se muestra la tabla de bloqueados
        gotoxy(0,18);
        printf("\tBloqueados\n\n#Programa    |    TTB");

        for(i=0;i<23;i++)
        {
            gotoxy(i,19);
            printf("_");
            gotoxy(i,26);
            printf("_");

        }
        if(l.proc[ejecutado].t.respuesta == -1)
            l.proc[ejecutado].t.respuesta=tiempo-l.proc[ejecutado].t.llegada;///tiempo de respuesta
        /******  Simula el tiempo ****/
        for( i=0; i <= restante && (toupper(tecla)!='I' && toupper(tecla)!='E' && toupper(tecla)!='S' ); i++ )
        {
            gotoxy(x2,y2+5);
            l.proc[ejecutado].tiempo_restante=restante-i;
            printf("Tiempo rest: %i ",l.proc[ejecutado].tiempo_restante);
            gotoxy(x2,y2+4);
            l.proc[ejecutado].tiempo_trans = l.proc[ejecutado].tiempo_max - l.proc[ejecutado].tiempo_restante;
            printf("Tiempo Trans: %i ",l.proc[ejecutado].tiempo_trans);
            if(l.proc[ejecutado].id!=-1)
            {
               gotoxy(x+18,7+ejecutado);
                printf("%i ",l.proc[ejecutado].tiempo_restante);
            }


            /** procesos bloqueados **/

            yb=21;//se inicializa la coordenada de escritura de los bloqueados
            for( j = 0 ; j < 5 ; j++)
            {
                gotoxy(1,yb+j);
                printf("   ");
                gotoxy(14,yb+j);
                printf("   ");
            }
            //se muestran los procesos bloqueados
            for( j=0; j<contBloqueados;j++ )
            {
                b.proc[j].tiempo_bloqueado+=1;
                if(b.proc[j].tiempo_bloqueado != 6)
                {
                    gotoxy(1,yb+j);
                    printf("%i", b.proc[j].id);
                    gotoxy(14,yb+j);
                    printf("%i", b.proc[j].tiempo_bloqueado);

                }

                if( b.proc[j].tiempo_bloqueado == 6)
                {

                    contBloqueados--;

                    b.proc[j].tiempo_bloqueado-=1;
                    aux=b.proc[j];
                    for( k=0 ; k<contBloqueados ; k++)
                    {
                        b.proc[k]=b.proc[k+1];

                    }

                    j--;
                    if(l.proc[0].id==-1)
                    {
                        contListos--;
                        bandbloqueo=0;
                        restante=aux.tiempo_restante;
                        i=0;
                    }
                    p[aux.id].estado=LISTO;
                    aux.estado=LISTO;
                    l.proc[contListos]=aux;
                    l.proc[ejecutado].estado=EJECUCION;
                    imprimirEjecucion(l,ejecutado,x2,y2);
                    contListos++;
                    actualizarListos(contListos,l,0,7);
                    imprimirMarcos(l,contListos,b,contBloqueados);

                }
            }

            gotoxy(5,30);
            printf("CONTADOR: %i    ",tiempo);
            Sleep(VEL);
            ///cambio de contexto
            if( i== quantum && tecla != 'I' && l.proc[ejecutado].tiempo_restante>0 && contListos >1 )
            {
                l.proc[ejecutado].estado=LISTO;
                aux=l.proc[ejecutado];

                for(j=0;j<contListos;j++)
                {
                    l.proc[j]=l.proc[j+1];
                }
                l.proc[j-1]=aux;
                mostrarListas(*num,cont,x,y,x2,x3,y2,y3,t,contTerminados,l,ejecutado,bandbloqueo,contListos,p,b,contBloqueados,tiempo,s,contSuspendido,contEspera);
                i=-1;
                l.proc[ejecutado].estado=EJECUCION;
                restante=l.proc[ejecutado].tiempo_restante;//variable aux para guardar el tiempo restante
                if(l.proc[ejecutado].t.respuesta == -1)
                {
                    l.proc[ejecutado].t.respuesta=tiempo-l.proc[ejecutado].t.llegada;///tiempo de respuesta
                }
                imprimirMarcos(l,contListos,b,contBloqueados);

            }
            //TECLAZOS
            do
            {
                if( kbhit() )
                {
                    tecla=getch();
                    switch(toupper(tecla))
                    {

                        case 'I'://interrupciones ---> bloqueo
                                //bloqueo

                                if(bandbloqueo!=1 )
                                {
                                    b.proc[contBloqueados]=l.proc[ejecutado];
                                    b.proc[contBloqueados].tiempo_bloqueado=0;
                                    p[b.proc[contBloqueados].id].estado=BLOQUEADO;
                                    contBloqueados++;
                                    bandInt=1;
                                    contListos--;
                                    if(contListos==0)
                                    {
                                      strcpy(l.proc[ejecutado].operacion,"null");
                                      l.proc[ejecutado].tiempo_max=5-b.proc[0].tiempo_bloqueado;
                                      l.proc[ejecutado].tiempo_restante=4-b.proc[0].tiempo_bloqueado;
                                      l.proc[ejecutado].id=-1;
                                      l.proc[ejecutado].tam=1;

                                      contListos++;
                                      bandbloqueo=1;
                                    }
                                    //system("cls");
                                }

                                if(contBloqueados==*num)
                                {

                                    l.proc[ejecutado].tiempo_restante=4-b.proc[0].tiempo_bloqueado;
                                }



                                break;
                        case 'E'://error
                                l.proc[ejecutado].estado=4;
                                break;
                        case 'A'://pause
                                do
                                {
                                    tecla=getch();
                                }while(toupper(tecla)!='O');//continuar
                                break;
                        case 'N'://proceso nuevo

                                p[*num].id=*num;//id automatico
                                creaOperacion(p[*num].operacion,&p[*num].resultado);
                                creaTiempo(&p[*num].tiempo_max);
                                creaTam(&p[*num].tam);
                                //marcos
                                p[*num].m.num=p[*num].tam/5;
                                if( p[*num].tam%5 != 0 )
                                {
                                    p[*num].m.num+=1;
                                }
                                p[*num].tiempo_restante=p[*num].tiempo_max;
                                p[*num].tiempo_trans=0;
                                p[*num].t.respuesta=-1;
                                p[*num].estado=NUEVO;//estado nuevo

                                *num+=1;
                                if( p[cont].m.num<=marcos_rest )
                                {
                                    for( i=0 ; i< p[cont].m.num ;i++)
                                    {
                                        marcos_rest--;
                                        p[cont].m.pos[i]=d.pos[marcos_rest];

                                    }
                                    p[*num-1].estado=LISTO;//estado listo
                                    if( l.proc[ejecutado].id==-1 )
                                    {
                                        contListos--;
                                        restante=p[*num-1].tiempo_restante;//variable aux para guardar el tiempo restante
                                        i=0;
                                        bandbloqueo=0;

                                    }
                                    l.proc[contListos]=p[*num-1];
                                    l.proc[contListos].t.llegada=tiempo;///tiempo de llegada
                                    l.proc[contListos].t.respuesta=tiempo-l.proc[contListos].t.llegada;///tiempo respuesta
                                    contListos++;
                                    cont++;
                                    system("cls");
                                    mostrarListas(*num,cont,x,y,x2,x3,y2,y3,t,contTerminados,l,ejecutado,bandbloqueo,contListos,p,b,contBloqueados,tiempo,s,contSuspendido,contEspera);

                                }

                                else
                                {
                                    gotoxy(10,1);
                                    printf("NUEVOS: %i  -->TAM: %i\t\tSUSPENDIDOS: %i  -->TAM: %i\t\tESPERA: %i\n",*num-cont,p[cont].tam,contSuspendido,s.proc[0].tam,contEspera);
                                }


                                break;
                        case 'B'://BCP
                                 mostrarBCP(p,cont,l,contListos,t,contTerminados,*num,tiempo,b,contBloqueados,s,contSuspendido);
                                 do
                                {
                                    tecla=getch();
                                }while(toupper(tecla)!='O');//continuar
                                mostrarListas(*num,cont,x,y,x2,x3,y2,y3,t,contTerminados,l,ejecutado,bandbloqueo,contListos,p,b,contBloqueados,tiempo,s,contSuspendido,contEspera);
                                break;
                        case 'T'://tabla paginas
                                mostrarTablaPaginas(p,*num);
                                do
                                {
                                    tecla=getch();
                                }while(toupper(tecla)!='O');//continuar
                                mostrarListas(*num,cont,x,y,x2,x3,y2,y3,t,contTerminados,l,ejecutado,bandbloqueo,contListos,p,b,contBloqueados,tiempo,s,contSuspendido,contEspera);
                                break;
                        case 'S'://suspendido
                                if(contBloqueados>0)
                                {
                                    b.proc[0].tiempo_bloqueado=0;
                                    b.proc[0].estado=SUSPENDIDO;
                                    p[b.proc[0].id].estado=SUSPENDIDO;
                                    s.proc[contSuspendido]=b.proc[0];

                                    contSuspendido++;

                                    //se libera memoria
                                    for( i=0 ;i< b.proc[0].m.num; i++)
                                    {
                                        d.pos[marcos_rest]=b.proc[0].m.pos[i];
                                        marcos_rest++;
                                    }

                                    for( i=0 ; i<contBloqueados-1; i++)
                                    {
                                        b.proc[i]=b.proc[i+1];
                                    }

                                    contBloqueados--;
                                    if( contSuspendido == (*num-contTerminados) )
                                    {
                                        bandSusp=0;
                                    }

                                }
                                break;
                        case 'R'://regresa a bloqueado
                                if( contSuspendido > 0 && contEspera<contSuspendido)
                                {
                                    if( s.proc[0].m.num <= marcos_rest )
                                    {
                                        s.proc[0].estado=BLOQUEADO;
                                        p[s.proc[0].id].estado=BLOQUEADO;
                                        b.proc[contBloqueados]=s.proc[0];
                                        //se asigna espacio en memoria
                                        for( i=0 ; i< b.proc[contBloqueados].m.num ;i++)
                                        {
                                            marcos_rest--;
                                            b.proc[contBloqueados].m.pos[i]=d.pos[marcos_rest];
                                            p[b.proc[contBloqueados].id].m.pos[i]=d.pos[marcos_rest];

                                        }
                                        contBloqueados++;
                                        for( i=0 ; i<contSuspendido-1; i++)
                                        {
                                            s.proc[i]=s.proc[i+1];
                                        }
                                        contSuspendido--;
                                        if(contSuspendido==0)
                                        {
                                            s.proc[0].tam=0;
                                            gotoxy(10,1);
                                            printf("NUEVOS: %i  -->TAM: %i\t\tSUSPENDIDOS: %i  -->TAM: 0  \t\tESPERA: %i\n",*num-cont,p[cont].tam,contSuspendido,contEspera);
                                        }

                                    mostrarListas(*num,cont,x,y,x2,x3,y2,y3,t,contTerminados,l,ejecutado,bandbloqueo,contListos,p,b,contBloqueados,tiempo,s,contSuspendido,contEspera);
                                    }
                                    else
                                    {

                                        contEspera++;
                                        gotoxy(10,1);
                                        printf("NUEVOS: %i  -->TAM: %i\t\tSUSPENDIDOS: %i  -->TAM: %i\t\tESPERA: %i\n",*num-cont,p[cont].tam,contSuspendido,s.proc[0].tam,contEspera);

                                    }


                                }


                                break;
                        default://nada
                                break;
                    }
                }//termina teclas
                if(contSuspendido == (*num-contTerminados))
                {
                    //se muestra contador
                    gotoxy(5,30);
                    printf("CONTADOR: %i    ",tiempo);
                    Sleep(VEL);
                    if( bandSusp==0 )
                    {
                        system("cls");
                        mostrarListas(*num,cont,x,y,x2,x3,y2,y3,t,contTerminados,l,ejecutado,bandbloqueo,contListos,p,b,contBloqueados,tiempo,s,contSuspendido,contEspera);
                        bandSusp=1;
                        timeSusp=0;
                    }
                    gotoxy(x2,y2+5);
                    printf("Tiempo rest: ? ");
                    gotoxy(x2,y2+4);
                    printf("Tiempo Trans: %i ",timeSusp);
                    timeSusp++;
                    i=0;

                }
                else
                {
                    bandSusp=0;
                }
                tiempo++;
            }while( contSuspendido == (*num-contTerminados) );

        }//fin del for
        if(bandInt!=1 )
            tiempo--;
        //corrimiento de la cola de listos
        if(bandbloqueo!=1 && toupper(tecla)!= 'S')
        {
            aux=l.proc[ejecutado];
            for(j=0;j<contListos;j++)
            {
                l.proc[j]=l.proc[j+1];

            }
            l.proc[ejecutado].estado=EJECUCION;
        }

        if(bandInt!=1 && bandbloqueo !=1 )
        {

            ejecutado--;

            //se libera memoria de los marcos ocupados por el proceso
            for( i=0 ;i< aux.m.num && toupper(tecla)!='S'; i++)
            {
                d.pos[marcos_rest]=aux.m.pos[i];
                marcos_rest++;
            }

            if(aux.id!=-1 && toupper(tecla)!='S')
            {
                p[aux.id].estado=TERMINADO;
                t[contTerminados]=aux;
                t[contTerminados].t.servicio=t[contTerminados].tiempo_max-t[contTerminados].tiempo_restante;///tiempo de servicio
                t[contTerminados].t.finalizacion=tiempo;///tiempo de finalizacion
                t[contTerminados].t.retorno=t[contTerminados].t.finalizacion-t[contTerminados].t.llegada;///tiempo de retorno
                t[contTerminados].t.espera=t[contTerminados].t.retorno-t[contTerminados].t.servicio;///tiempo de espera

                if(bandbloqueo==0)
                    contTerminados++;//contador de terminados

                if(contListos-1==0 && contSuspendido>0)
                {

                  strcpy(l.proc[ejecutado].operacion,"null");
                  l.proc[ejecutado+1].tiempo_max=5;
                  l.proc[ejecutado+1].tiempo_restante=5;
                  l.proc[ejecutado+1].id=-1;
                  l.proc[ejecutado+1].tam=1;
                  contListos=2;
                }
            }

            if(toupper(tecla)!='S')
                contListos--;

            ejecutado++;//contador del proceso

            /**** si hay aun procesos se ponen en listos ****/
            while(contEspera>0 && s.proc[0].m.num <= marcos_rest && toupper(tecla)!='S')
            {
                b.proc[contBloqueados]=s.proc[0];
                //se asigna espacio en memoria
                for( i=0 ; i< b.proc[contBloqueados].m.num ;i++)
                {
                    marcos_rest--;
                    b.proc[contBloqueados].m.pos[i]=d.pos[marcos_rest];
                    p[b.proc[contBloqueados].id].m.pos[i]=d.pos[marcos_rest];

                }
                contBloqueados++;
                for( i=0 ; i<contSuspendido-1; i++)
                {
                    s.proc[i]=s.proc[i+1];
                }
                contSuspendido--;
                contEspera--;
            }
            while(cont < *num &&  bandbloqueo==0 && l.proc[ejecutado].id!=-1 && p[cont].m.num<=marcos_rest && contEspera==0)
            {
                for( i=0 ; i< p[cont].m.num ;i++)
                {
                    marcos_rest--;
                    p[cont].m.pos[i]=d.pos[marcos_rest];

                }

                p[cont].estado=LISTO;//estado listo
                l.proc[contListos]=p[cont];
                l.proc[contListos].t.llegada=tiempo;///tiempo de llegada
                contListos++;
                cont++;
            }


        }
        bandInt=0;

    }//fin del while

    //se borra ejecucion
    gotoxy(x2,y2);
    printf("\t\t");
    gotoxy(x2,y2+1);
    printf("\t\t");
    gotoxy(x2,y2+2);
    printf("\t\t");
    gotoxy(x2,y2+3);
    printf("\t\t");
    gotoxy(x2,y2+4);
    printf("\t\t");
    gotoxy(x2,y2+5);
    printf("\t\t");

    //se muestra el ultimo proceso
    draw(x,x2,y3);
    gotoxy(x3,y3);
    printf("%i          | %s  =  %lf",t[contTerminados-1].id,t[contTerminados-1].operacion,t[contTerminados-1].resultado);

    //se borra el ultimo en listos
    gotoxy(0,7);
    printf("    ");
    gotoxy(11,7);
    printf("  ");
    gotoxy(18,7);
    printf("  ");
    //se muestra contador
    gotoxy(5,30);
    printf("CONTADOR: %i    ",tiempo);

}

void creaOperacion(String operacion,double *resultado)
{
    int signo,num1,num2;
    double num3,num4;
    String temp;

    signo=rand()%6+1;

    num1=rand()%99+1;
    num2=rand()%99+1;
    //numero exclusibos para porcentaje y aprovechar el punto decimal
    num3=num1;
    num4=num2;
    switch(signo)
    {
        case 1://suma
                sprintf(operacion,"%d",num1);
                strcat(operacion,"+");
                sprintf(temp,"%d",num2);
                strcat(operacion,temp);
                *resultado=num1+num2;
                break;
        case 2://resta
                sprintf(operacion,"%d",num1);
                strcat(operacion,"-");
                sprintf(temp,"%d",num2);
                strcat(operacion,temp);
                *resultado=num1-num2;
                break;
        case 3://division
                sprintf(operacion,"%d",num1);
                strcat(operacion,"/");
                sprintf(temp,"%d",num2);
                strcat(operacion,temp);
                *resultado=num3/num4;
                break;
        case 4://multiplicacion
                sprintf(operacion,"%d",num1);
                strcat(operacion,"*");
                sprintf(temp,"%d",num2);
                strcat(operacion,temp);
                *resultado=num1*num2;
                break;
        case 5://porcentaje
                sprintf(operacion,"%d",num1);
                strcat(operacion,"#");
                sprintf(temp,"%d",num2);
                strcat(operacion,temp);
                *resultado=num3*(num4/100);
                break;
        case 6://modulo
                sprintf(operacion,"%d",num1);
                strcat(operacion,"%");
                sprintf(temp,"%d",num2);
                strcat(operacion,temp);
                *resultado=num1%num2;
                break;
        default:
                break;

    }

}
void creaTiempo(unsigned int *tiempo)
{
    *tiempo=rand()%20+1;

}
void creaTam( unsigned *tam )
{
    *tam=rand()%50+1;
}

int inicializarListos(struct proceso p[],struct listos *l,unsigned int *contListos,unsigned int num ,Disponibles *d)
{
    unsigned int i,j,cont=0,band=0,marcos=0;

    for( i=0 ; band==0 && i<num ;i++ )
    {
        marcos+=p[i].m.num;
//        printf("p[%i].marcos: %i\n\n",i,p[i].m.num);
        if( ( NUM_MARCOS/marcos)!=0 )
        {
            for( j=0 ; j< p[i].m.num ; j++)
            {
                p[i].m.pos[j]=cont;
                cont++;
            }

            *contListos+=1;
        }
        else
        {
            marcos-=p[i].m.num;
            band=1;
        }
    }
    (*d).num=NUM_MARCOS-cont;
    for( j=0 ;cont<NUM_MARCOS; cont++,j++  )
    {
        //disponibles
        (*d).pos[j]=cont;

    }
    for( i=0 ; i<*contListos && i<num; i++)
    {
        p[i].estado=LISTO;//estado listo
        if( i == 0)
            p[i].estado=EJECUCION;

        (*l).proc[i]=p[i];
        (*l).proc[i].t.llegada=0;
    }
    /*for( i=0 ; i < *contListos ; i++)
        for( j=0 ; j< p[i].m.num ; j++)
        {
            printf("\np[%i].pos[%i]: %i",i,j,p[i].m.pos[j]);
        }*/
    return NUM_MARCOS-marcos;
}
void actualizarListos(unsigned int contListos,struct listos l,int x,int y)
{
    unsigned int i;


    for(i=0;i<contListos;i++)
    {

        gotoxy(x,y);
        printf("  %i  ",l.proc[i].id);
        if(l.proc[i].id != -1)
        {
            gotoxy(x+11,y);
            printf("%i",l.proc[i].tiempo_max);
            gotoxy(x+18,y);
            printf("%i ",l.proc[i].tiempo_restante);
        }

        y++;
    }

}
void draw_timeTable(struct proceso ter[],unsigned int num)
{
    unsigned int i,j,y=5;

    printf("\n\t\t\t\t\t******** TIEMPOS ********\n");
    for( i=0 ;i<95 ;i++)
        printf("_");

    printf("\nID  |  OPERACION           | LLEGADA | FINALIZACION | RETORNO | RESPUESTA | ESPERA | SERVICIO |\n");
    for( i=0 ;i<95 ;i++)
        printf("_");
    for(i=0;i<num;i++)
    {
        gotoxy(2,y);
        printf("%i",ter[i].id);
        gotoxy(7,y);
        printf("%s = ",ter[i].operacion);
        if( ter[i].estado!=4 )//estado error
            printf("%lf",ter[i].resultado);
        else
            printf("ERROR");
        gotoxy(33,y);
        printf("%i",ter[i].t.llegada);
        gotoxy(44,y);
        printf("%i",ter[i].t.finalizacion);
        gotoxy(57,y);
        printf("%i",ter[i].t.retorno);
        gotoxy(68,y);
        printf("%i",ter[i].t.respuesta);
        gotoxy(79,y);
        printf("%i",ter[i].t.espera);
        gotoxy(89,y);
        printf("%i",ter[i].t.servicio);
        printf("\n");
        for( j=0 ;j<95 ;j++)
        printf("_");
        y+=2;
    }
    for( i=4 ; i<y ; i++)
    {
        gotoxy(4,i);
        printf("|");
        gotoxy(27,i);
        printf("|");
        gotoxy(37,i);
        printf("|");
        gotoxy(52,i);
        printf("|");
        gotoxy(62,i);
        printf("|");
        gotoxy(74,i);
        printf("|");
        gotoxy(83,i);
        printf("|");
        gotoxy(94,i);
        printf("|");
    }
}
void imprimirMarcos(struct listos l , unsigned int contListos , struct listos b, unsigned int contBloqueados)
{
    unsigned int j,contador=0,k,h;

    dibujaTablaMarco();

    for(k=0;k<contListos && l.proc[k].id != -1;k++)
    {

        h=0;
        contador=0;
        for( j=38 ; contador < l.proc[k].tam; j+=3 )
        {
            gotoxy( 3+(l.proc[k].m.pos[h]*4) , j);
            if( l.proc[k].estado==LISTO )
            {
                printf("%iL",l.proc[k].id);
            }
            else if( l.proc[k].estado== EJECUCION )
                {
                    printf("%iE",l.proc[k].id);
                }

            contador++;
            if(contador%5==0)
            {
                j=35;
                h++;
            }
        }

    }
    for(k=0;k<contBloqueados;k++)
    {
        h=0;
        contador=0;
        for( j=38 ; contador < b.proc[k].tam; j+=3 )
        {
            gotoxy( 3+(b.proc[k].m.pos[h]*4) , j);
            printf("%iB ",b.proc[k].id);
            contador++;
            if(contador%5==0)
            {
                j=35;
                h++;
            }
        }

    }



}
void imprimirEjecucion(struct listos l,unsigned int ejecutado,unsigned int x2,unsigned int y2)
{

        gotoxy(x2,y2);
        printf("Operacion: %s",l.proc[ejecutado].operacion);
        gotoxy(x2,y2+1);
        printf("TME: %i",l.proc[ejecutado].tiempo_max);
        gotoxy(x2,y2+2);
        printf("No.Programa: %i  \n",l.proc[ejecutado].id);
}
void mostrarBCP(struct proceso p[],unsigned int cont,struct listos l,unsigned int contListos,struct proceso t[],unsigned int contTerminados, unsigned int num,unsigned int tiempo,struct listos b,unsigned int contBloqueados,struct listos s,unsigned int contSuspendido)
{
    unsigned int i,x=2,y=4;

    system("cls");
    printf("\t\t\t\t\t\t***** BCP *****\n");

    dibujaLinea();

    printf("\n ID |  Estado  |         Operacion       | Llegada | Finalizacion | Retorno | Espera | Servicio | Restante | Respuesta | Bloqueado |\n");
            //0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012
            //          10          20      30          40      50          60      70          80      90          100     110       120
    dibujaLinea();
    ///se muestran datos de TERMINADOS
    for( i = 0 ; i < contTerminados ; i++)
    {
        gotoxy(x,y);
        printf("%i",t[i].id);
        gotoxy(x+4,y);
        if(t[i].estado !=4)
        {
            printf("Terminado");//estado
            gotoxy(x+17,y);
            printf("%s = %lf",t[i].operacion,t[i].resultado);
        }

        else
        {
            printf("ERROR");//estado
            gotoxy(x+17,y);
            printf("%s = ERROR",t[i].operacion);
        }


        gotoxy(x+41,y);
        printf("%i",t[i].t.llegada);
        gotoxy(x+53,y);
        printf("%i",t[i].t.finalizacion);//finalizacion
        gotoxy(x+66,y);
        printf("%i",t[i].t.retorno);//retorno
        gotoxy(x+76,y);
        printf("%i",t[i].t.espera);//espera
        gotoxy(x+86,y);
        printf("%i",t[i].tiempo_max);//servicio
        gotoxy(x+96,y);
        printf("%i",t[i].tiempo_restante);
        gotoxy(x+107,y);
        printf("%i",t[i].t.respuesta);
        gotoxy(x+121,y);
        printf("NULL\n");
        dibujaLinea();
        y+=2;
    }
    ///se muestran datos de BLOQUEADOS
    for( i = 0 ; i < contBloqueados ; i++)
    {
        gotoxy(x,y);
        printf("%i",b.proc[i].id);
        gotoxy(x+4,y);
        printf("Bloqueado");//estado
        gotoxy(x+17,y);
        printf("%s = NULL",b.proc[i].operacion);
        gotoxy(x+41,y);
        printf("%i",b.proc[i].t.llegada);
        gotoxy(x+53,y);
        printf("NULL");//finalizacion
        gotoxy(x+66,y);
        printf("NULL");//retorno
        gotoxy(x+76,y);
        printf("%i",(tiempo-b.proc[i].t.llegada)-(b.proc[i].tiempo_max-b.proc[i].tiempo_restante));//espera
        gotoxy(x+86,y);
        printf("%i",b.proc[i].tiempo_max);//servicio
        gotoxy(x+96,y);
        printf("%i",b.proc[i].tiempo_restante);
        gotoxy(x+107,y);
        printf("%i",b.proc[i].t.respuesta);
        gotoxy(x+121,y);
        printf("%i\n",b.proc[i].tiempo_bloqueado);
        dibujaLinea();
        y+=2;
    }
    ///se muestran datos de SUSPENDIDOS
    for( i = 0 ; i < contSuspendido ; i++)
    {
        gotoxy(x,y);
        printf("%i",s.proc[i].id);
        gotoxy(x+3,y);
        printf("Suspendido");//estado
        gotoxy(x+17,y);
        printf("%s = NULL",s.proc[i].operacion);
        gotoxy(x+41,y);
        printf("%i",s.proc[i].t.llegada);
        gotoxy(x+53,y);
        printf("NULL");//finalizacion
        gotoxy(x+66,y);
        printf("NULL");//retorno
        gotoxy(x+76,y);
        printf("%i",(tiempo-s.proc[i].t.llegada)-(s.proc[i].tiempo_max-s.proc[i].tiempo_restante));//espera
        gotoxy(x+86,y);
        printf("%i",s.proc[i].tiempo_max);//servicio
        gotoxy(x+96,y);
        printf("%i",s.proc[i].tiempo_restante);
        gotoxy(x+107,y);
        printf("%i",s.proc[i].t.respuesta);
        gotoxy(x+121,y);
        printf("%i\n",s.proc[i].tiempo_bloqueado);
        dibujaLinea();
        y+=2;
    }
    ///se muestran datos de LISTOS
    for( i = 0 ; i < contListos ; i++)
    {
        if( l.proc[i].id != -1 )
        {
            gotoxy(x,y);
            printf("%i",l.proc[i].id);
            gotoxy(x+3,y);
            if(i==0)
            {
                printf("Ejecucion");//estado
            }
            else
                printf("Listo");//estado
            gotoxy(x+17,y);
            printf("%s = NULL",l.proc[i].operacion);
            gotoxy(x+41,y);
            printf("%i",l.proc[i].t.llegada);
            gotoxy(x+53,y);
            printf("NULL");//finalizacion
            gotoxy(x+66,y);
            printf("NULL");//retorno
            gotoxy(x+76,y);
            printf("%i",(tiempo-l.proc[i].t.llegada)-(l.proc[i].tiempo_max-l.proc[i].tiempo_restante));//espera
            gotoxy(x+86,y);
            printf("%i",l.proc[i].tiempo_max);//servicio
            gotoxy(x+96,y);
            printf("%i",l.proc[i].tiempo_restante);
            gotoxy(x+107,y);
            if( l.proc[i].t.respuesta != -1 )
            {
                printf("%i",l.proc[i].t.respuesta);
            }
            else
            {
                printf("NULL");//respuesta
            }
            gotoxy(x+121,y);
            printf("NULL\n");
            dibujaLinea();
            y+=2;
        }

    }

    ///se muestran datos de NUEVOS
    for( i = cont ; i < num ; i++)
    {
        gotoxy(x,y);
        printf("%i",p[i].id);
        gotoxy(x+5,y);
        printf("Nuevo");//estado
        gotoxy(x+17,y);
        printf("%s = NULL",p[i].operacion);
        gotoxy(x+41,y);
        printf("NULL");//llegada
        gotoxy(x+53,y);
        printf("NULL");//finalizacion
        gotoxy(x+66,y);
        printf("NULL");//retorno
        gotoxy(x+76,y);
        printf("NULL");//espera
        gotoxy(x+86,y);
        printf("%i",p[i].tiempo_max);//servicio
        gotoxy(x+96,y);
        printf("%i",p[i].tiempo_restante);
        gotoxy(x+107,y);
        printf("NULL\n");//respuesta
        gotoxy(x+121,y);
        printf("NULL\n");
        dibujaLinea();
        y+=2;
    }

    for( i = 4 ; i<y ; i+=2)
    {
        gotoxy(4,i);
        printf("|");
        gotoxy(15,i);
        printf("|");
        gotoxy(41,i);
        printf("|");
        gotoxy(51,i);
        printf("|");
        gotoxy(66,i);
        printf("|");
        gotoxy(76,i);
        printf("|");
        gotoxy(85,i);
        printf("|");
        gotoxy(96,i);
        printf("|");
        gotoxy(107,i);
        printf("|");
        gotoxy(119,i);
        printf("|");
        gotoxy(131,i);
        printf("|");

    }
}
void mostrarListas(unsigned int num,unsigned int cont,unsigned int x,unsigned int y,unsigned int x2,unsigned int x3,unsigned int y2,unsigned int y3,struct proceso t[],unsigned int contTerminados,struct listos l,unsigned int ejecutado,unsigned int bandbloqueo, unsigned int contListos,struct proceso p[],struct listos b, unsigned int contBloqueados,unsigned int tiempo, struct listos s, unsigned int contSuspendido,unsigned int contEspera)
{
    unsigned int i;

    system("cls");
    imprimirMarcos(l,contListos,b,contBloqueados);
    gotoxy(10,1);
    printf("NUEVOS: %i  -->TAM: %i\t\tSUSPENDIDOS: %i  -->TAM: %i\t\tESPERA: %i\n",num-cont,p[cont].tam,contSuspendido,s.proc[0].tam,contEspera);

    printf("\n\n\tListos\t\t  Proceso en Ejecucion\t\t\tTerminados\n");
    for(i=0;i<95;i++)
        printf("_");
            //se inicializan las coordenadas
        x=0;
        y=6;
        x2=32;
        y2=6;
        x3=60;
        y3=6;
    //se dibuja la tabla
    draw(x,x2,y3);

    //se muestra procesos terminados
    gotoxy(x3-3,y3);
    printf("#Programa     |\tOperacion");
    y3++;
    for(i=0;i<contTerminados;i++)
    {
        gotoxy(x3,y3);
        printf("%i\t\t %s  =",t[i].id,t[i].operacion);
        if(t[i].estado!=4)//estado error
        {
            printf("  %lf  ",t[i].resultado);
        }

        else
            printf("  ERROR");

        y3++;
        draw(x,x2,y3);
    }
    //se muestra procesos listos
    gotoxy(x,y);
    printf("#Programa| TME  | TRE");
    if(bandbloqueo!=1)
    {
        y++;
        actualizarListos(contListos,l,x,y);
    }

    //se muestra proceso en ejecucion

    imprimirEjecucion(l,ejecutado,x2,y2);
    gotoxy(x2,y2+5);
    printf("Tiempo rest: %i ",l.proc[ejecutado].tiempo_restante);
    gotoxy(x2,y2+4);
    l.proc[ejecutado].tiempo_trans = l.proc[ejecutado].tiempo_max - l.proc[ejecutado].tiempo_restante;
    printf("Tiempo Trans: %i ",l.proc[ejecutado].tiempo_trans);
    //opciones de teclas
    gotoxy(30,18);
    printf("- TABLA PAGINAS:  T");
    gotoxy(30,20);
    printf("- INTERRUPCION:   I");
    gotoxy(30,22);
    printf("- ERROR:          E");
    gotoxy(30,24);
    printf("- PAUSA:          A");
    gotoxy(30,26);
    printf("- CONTINUAR:      O");
    gotoxy(30,28);
    printf("- PROCESO NUEVO:  N");
    gotoxy(30,30);
    printf("- BCP:            B");


    //se muestra la tabla de bloqueados
    gotoxy(0,18);
    printf("\tBloqueados\n\n#Programa    |    TTB");

    for(i=0;i<23;i++)
    {
        gotoxy(i,19);
        printf("_");
        gotoxy(i,26);
        printf("_");
    }

    gotoxy(5,30);
    printf("CONTADOR: %i    ",tiempo);
}
void mostrarTablaPaginas(struct proceso p[] ,unsigned int num)
{
    unsigned int x,y,i,j;

    system("cls");
    printf("\n\t\t*****  Tabla de Paginas  ******");
    //se dibuja tabla
    for( x = 0 , i=0 ; x < (num*4) ; x+=4 , i++ )
    {
        gotoxy( 10+x , 5 );
        printf("%i",i);
    }
    for(y=8;y<53;y+=4)
        for( x=8; x<(num*4)+9 ; x++)
        {
            gotoxy(x,y);
            printf("-");

        }
    for( x=8 ; x<(num*4)+12;x+=4 )
        for( y=8 ; y<53 ; y++ )
        {
            gotoxy(x,y);
            if(y%4!=0)
                printf("|");
            else
                printf("+");
        }
    i=0;
    for( x=10 ; x<(num*4)+10;x+=4 )
    {
        j=0;
        for( y=10 ; y<53; y+=4 )
        {
            gotoxy(x,y);
            if( j < p[i].m.num && (p[i].estado == LISTO || p[i].estado == BLOQUEADO  || p[i].estado == EJECUCION ) )
            {
                printf("%i",p[i].m.pos[j]);
                j++;
            }
            else
                printf("%c",177);

        }

        i++;
    }


}
#endif // OPERACIONES_H_INCLUDED
