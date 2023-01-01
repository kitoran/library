#include "persistent.h"
#include "settings.h"
#include "toolbuttongroup.h"
#include "loadImage.h"
#include "stb_ds.h"
#include "stdio.h"

typedef struct Unit {} Unit;
static Unit unit;
bool persistentIntField_(Painter*p, int digits, int* number, char* name) {
    static struct {
        char* key;
        Unit value;
    } *map = NULL;

    int index = shgeti(map, name);
    if(index == -1) {
        bool success;
        int value = loadInt(name, &success);
        if(success) {
            *number = value;
        }
        shput(map, name, unit);
    }
    if(guiIntField(p, digits, number)) {
        saveInt(name, *number);
        return true;
    }
    return false;
}
bool persistentDoubleField_(Painter*p, int digits, double* number, char* name) {
    static struct {
        char* key;
        Unit value;
    } *map = NULL;

    int index = shgeti(map, name);
    if(index == -1) {
        bool success;
        double value = loadDouble(name, &success);
        if(success) {
            *number = value;
        }
        shput(map, name, unit);
    }
    if(guiDoubleField(p, digits, number)) {
        saveDouble(name, *number);
        return true;
    }
    return false;
}
bool standardResourseToolButton(Painter*p, char* name) {
    static struct {
        char* key;
        IMAGE* value;
    } *map = NULL;

    IMAGE* image;
    int index = shgeti(map, name);
    if(index == -1) {
        image = loadImageZT(GUI_RESOURCE_PATH, name);
        shput(map, name, image);
    } else {
        image = map[index].value;
    }
    return guiToolButton(p, image);
}
bool resourseToolButton(Painter*p, const char* name) {
   return resourseToolButtonEx(p, name, false, NULL);
}
bool resourseToolButtonEx(Painter*p, const char* name, bool active, Size* desirableSize) {
    static struct {
        char* key;
        IMAGE* value;
    } *map = NULL;

    IMAGE* image;
    int index = shgeti(map, name);
//    fprintf(stderr, "%d, index", index);
    if(index == -1) {
        image = loadLocalImageZT(name);
        shput(map, name, image);
    } else {
        image = map[index].value;
    }
    return guiToolButtonEx(p, image, active, desirableSize);
}

bool persistentComboBoxZT_(Painter *p, const char * const *elements, int *current, char *name)
{
    static struct {
        char* key;
        Unit value;
    } *map = NULL;

    int index = shgeti(map, name);
    if(index == -1) {
        bool success;
        int value = loadInt(name, &success);
        if(success) {
            *current = value;
        }
        shput(map, name, unit);
    }
    if(guiComboBoxZT(p, elements, current)) {
        saveInt(name, *current);
        return true;
    }
    return false;
}
bool persistentToolButtonGroup_(Painter *p, int *v, const char*const*  filenames, int enumSize, char* saveName) {
    static struct {
        char* key;
        Unit value;
    } *map = NULL;

    int index = shgeti(map, saveName);
    if(index == -1) {
        bool success;
        int value = loadInt(saveName, &success);
        if(success) {
            *v = value;
        }
        shput(map, saveName, unit);
    }
    if(guiToolButtonGroup_(p,v,filenames,enumSize)) {
        saveInt(saveName, *v);
        return true;
    }
    return false;
}
