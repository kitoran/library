#ifndef TOOLBUTTONGROUP_H
#define TOOLBUTTONGROUP_H
//struct Painter;
#include <backend.h>
_Bool guiToolButtonGroup_(/*struct */Painter* p, int* enumv, const char*const enumNames[], int enumSize);
//_Bool guiToolButtonGroup(struct Painter* p, int* enumv, char* enumNames[], int enumSize);
#define guiToolButtonGroup(p, e, v) \
    guiToolButtonGroup_(p, (int*)v, e ## Filenames, e## Size)

_Bool guiRadioButtonGroup_(Painter* p, int* enumv, const char*const names[], int enumSize);
#define guiRadioButtonGroup(p, e, v) \
    guiRadioButtonGroup_(p, (int*)v, e ## Names, e## Size)

#endif // TOOLBUTTONGROUP_H
