#include "persistent.h"
#include "settings.h"
#include "stb_ds.h"
typedef struct Unit {} Unit;
static Unit unit;
bool persistentNumberEdit_(Painter*p, int digits, int* number, char* name) {
    static struct {
        char* key;
        Unit value;
//        struct {
//            bool initialized;
//        } value;
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
    if(guiNumberEdit(p, digits, number)) {
        saveInt(name, *number);
        return true;
    }
    return false;
}

