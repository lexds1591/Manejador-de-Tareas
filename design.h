/*
 *design.h
 *Functions for creating tables
 *created by Alejandro Duarte Sánchez
 *version 05/10/2012
*/

#ifndef DESIGN_H_INCLUDED
#define DESIGN_H_INCLUDED

int gotoxy(short x, short y);
void draw(int x,int x2,int y3);
void dibujaLinea(void);
void dibujaTablaMarco(void);

int gotoxy(short x, short y)
{
   COORD coord;
   HANDLE h_stdout;

   coord.X = x;
   coord.Y = y;

   if ((h_stdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
      return 0;

   if (SetConsoleCursorPosition(h_stdout, coord) == 0)
      return 0;

   return 1;
}
void draw(int x,int x2,int y3)
{
    int j;

    for(j=6;j<13;j++)
    {
        gotoxy(x+22,j);
        printf("|");
        gotoxy(9,j);
        printf("|");
        gotoxy(16,j);
        printf("|");
        gotoxy(x2+22,j);
        printf("|");
        gotoxy(13,j+14);
        printf("|");
        gotoxy(22,j+14);
        printf("|");
    }

    /*for( j = 0 ; j < 55 ; j++ )
    {
        gotoxy(x+j,12);
        printf("_");
    }*/
    for(j=6;j<=y3;j++)
    {
        gotoxy(x2+22,j);
        printf("|");

        gotoxy(x2+39,j);
        printf("|");

        gotoxy(95,j);
        printf("|");


    }
}
void dibujaLinea(void)
{
    int i;

    for( i = 0 ; i < 132 ; i++ )
    {
        if( i!=4  && i!= 15 && i!= 41 && i!= 51 && i!= 66 && i!= 76 && i!= 85 && i!= 96 && i!= 107 && i!= 119 && i!= 131 )
        {
            printf("-");
        }
        else
        {
            printf("+");
        }

    }
}
void dibujaTablaMarco(void)
{
    int i,j;

    for( i=0 , j=0 ;i<=103;i+=4,j++ )
    {

        gotoxy(4+i,35);
        printf("%i",j);


    }
    for(i=0 ; i<15; i++)
    {
        gotoxy(104,37+i);
        printf("%c",177);
    }

    for( j=0 ; j<=104 ; j+=4)
        for( i=0;i<16 ; i++)
        {
            gotoxy(2+j,36+i);
            printf("|");

        }
    for( j=0 ; j< 18; j+=3)
        for( i=0 ; i<=104;i++ )
        {

            if( i%4!=0 )
            {
                gotoxy(2+i,36+j);
                printf("-");
            }

            else
            {
                gotoxy(2+i,36+j);
                printf("+");

            }
        }
}
#endif // DESIGN_H_INCLUDED
