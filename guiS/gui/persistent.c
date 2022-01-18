#include "persistent.h"


bool persistentNumberEdit_(Painter*p, int digits, int* number, char* name) {
    static struct {
        char* key;
        struct {} value;
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
        shput(map, name, *number);
    }
    if(guiNumberEdit(p, digits, number)) {
        storeInt(name, *number);
    }
}

