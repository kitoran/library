#include "extmath.h"
#include "math.h"
#include "assert.h"
#include "inttypes.h"
#include <stdbool.h>


const double tau = 6.283185307179586476925286766559005768394338798750211641949889184615632812572417997256069650684234135964296173026564613294187689219101164463450718816256962234900568205403;
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

//void convergents(double s, int* res, int n) {
//    assert(n == 3);
//    double intPart = round(s);
//    res[0] = intPart; res[1] = 1;
//    s = 1/(s-intPart);
//}
