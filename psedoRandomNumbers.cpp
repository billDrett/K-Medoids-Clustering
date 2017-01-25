#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "psedoRandomNumbers.h"
void initialize() //set thn srand
{
    srand(time(NULL));
}

double Marsaglia() //gauss kanonikh metavliti
{
    double y1, y2, s;

    do
    {
        y1 = randomNumberDouble(-1,1); //einai anoixto diastima ara dn theloume thn timh -1
        y2 = randomNumberDouble(-1,1);
        s = y1*y1+y2*y2;
    }while(s == 0 || s>=1); //s oxi mhden giati mpainei ston paranomasth

    return y1*sqrt((-2* log(s))/s);
}

double randomNumberDouble(double from, double to)
{
    return from + ((double)rand() / RAND_MAX)*(to-from);
}

int randomNumberInt(int from, int to) //epistrefei akeraio mesa sto kleisto diastima
{
    return from + rand()%(int)(to-from+1);
}
