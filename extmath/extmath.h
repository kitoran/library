#ifndef EXTMATH_H
#define EXTMATH_H


#define LOOKUP_POWER 15
#define LOOKUP_TABLE_SIZE (1 << LOOKUP_POWER)

void initializeMathExt();

double fastSin(double x);//should be inline
int fastReducedSinInt(int x); //should be inline
extern double* sinLookupTable;
extern int* sinLookupTableInt;
extern const double tau;
int intLog2(int l);
static inline unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void convergents(double s, int* res, int n);

#endif // EXTMATH_H
