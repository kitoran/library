#include "extmath.h"
#include "math.h"
#include "assert.h"
#include "inttypes.h"
#include <stdbool.h>


const double tau = M_PI*2;
double *makeSinLookupTable()
{
    static bool initialized = false;
    static double lookup[LOOKUP_TABLE_SIZE];
    if(!initialized) {
        for(int i = 0; i < LOOKUP_TABLE_SIZE; i++) {
          lookup[i] = /*(*/sin(i*tau/LOOKUP_TABLE_SIZE)/*/2+1)*UINT16_MAX*/;
        }
        initialized = true;
    }
    return lookup;
}
double *sinLookupTable = 0;
int * makeSinLookupTableInt()
{
    static bool initialized = false;
    static int lookup[LOOKUP_TABLE_SIZE];
    if(!initialized) {
        for(int i = 0; i < LOOKUP_TABLE_SIZE; i++) {
          lookup[i] = /*(*/sin(i*tau/LOOKUP_TABLE_SIZE)/*/2+1)*/*INT16_MAX;
        }
        initialized = true;
    }
    return lookup;
}
int* sinLookupTableInt = 0;

inline double fastSin(double x)
{
    return sinLookupTable
            [(int)(x/tau*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE];
}

int intLog2(int l) {
    int res = 0;
    while(l >>= 1) {
        res++;
    }
    return res;
}


void initializeMathExt()
{
    sinLookupTable = makeSinLookupTable();
    sinLookupTableInt = makeSinLookupTableInt();
}

void convergents(double s, int* res, int n) {
    assert(n == 3);
    double intPart = round(s);
    res[0] = intPart; res[1] = 1;
    s = 1/(s-intPart);
}
