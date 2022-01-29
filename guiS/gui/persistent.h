#ifndef PERSISTENT_H
#define PERSISTENT_H
#include <gui.h>


bool persistentNumberEdit_(Painter*p, int digits, int* number, char* name);
#define persistentNumberEdit(p, d, n) \
    persistentNumberEdit_(p, d, n, #n)

bool resourseToolButton(Painter*p, char* name);
#endif // PERSISTENT_H
