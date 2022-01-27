#ifndef PERSISTENT_H
#define PERSISTENT_H
#include <gui.h>


bool persistentNumberEdit_(Painter*p, int digits, int* number, char* name);


//#define STRINGIFY1(x) #x
//#define STRINGIFY(x) STRINGIFY1(x)
#define persistentNumberEdit(p, d, n) \
    persistentNumberEdit_(p, d, n, #n)

#endif // PERSISTENT_H
