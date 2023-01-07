#ifndef PERSISTENT_H
#define PERSISTENT_H
#include <gui.h>

#ifndef GUI_NO_PERSISTENT_WIDGETS
bool persistentIntField_(Painter*p, int digits, int* number, char* name);
#define persistentIntField(p, d, n) \
    persistentNumberEdit_(p, d, (int*)&n, #n)bool persistentNumberEdit_(Painter*p, int digits, int* number, char* name);
bool persistentDoubleField_(Painter*p, int digits, double* number, char* name);
#define persistentDoubleField(p, d, n) \
    persistentDoubleField_(p, d, (double*)&n, #n)
bool persistentComboBoxZT_(Painter*p, char const*const* elements, int *current, char* name);
#define persistentEnumComboBox(enum, p, c) \
    persistentComboBoxZT_(p, enum ## Names, &c, #c)
bool persistentToolButtonGroup_(Painter *p, int *v, const char*const* filenames, int enumSize, char* saveName);
#define persistentToolButtonGroup(p, e, c) \
    persistentToolButtonGroup_(p, (int*)&c, e ## Filenames, e## Size, #c)
#else
#define persistentIntField(p, d, n) guiIntField(p, d, &n)
#define persistentDoubleField(p, d, n) guiDoubleField(p, d, &n)
#define persistentEnumComboBox(p, d, n) guiEnumComboBox(p, d, &n)
#define persistentToolButtonGroup(p, d, n) guiToolButtonGroup(p, d, &n)
#endif
#endif // PERSISTENT_H
