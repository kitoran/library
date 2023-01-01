#ifndef PERSISTENT_H
#define PERSISTENT_H
#include <gui.h>


bool persistentIntField_(Painter*p, int digits, int* number, char* name);
#define persistentIntField(p, d, n) \
    persistentNumberEdit_(p, d, (int*)&n, #n)bool persistentNumberEdit_(Painter*p, int digits, int* number, char* name);
bool persistentDoubleField_(Painter*p, int digits, double* number, char* name);
#define persistentDoubleField(p, d, n) \
    persistentDoubleField_(p, d, (double*)&n, #n)
bool resourseToolButtonEx(Painter*p, const char* name, bool active, Size *desirableSize);
bool resourseToolButton(Painter*p, const char* name);
bool standardResourseToolButton(Painter*p, char* name);
bool persistentComboBoxZT_(Painter*p, char const*const* elements, int *current, char* name);
#define persistentEnumComboBox(enum, p, c) \
    persistentComboBoxZT_(p, enum ## Names, &c, #c)
bool persistentToolButtonGroup_(Painter *p, int *v, const char*const* filenames, int enumSize, char* saveName);
#define persistentToolButtonGroup(p, e, c) \
    persistentToolButtonGroup_(p, (int*)&c, e ## Filenames, e## Size, #c)

#endif // PERSISTENT_H
