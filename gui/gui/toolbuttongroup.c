#include "toolbuttongroup.h"
#include "stdbool.h"
#include "gui.h"
#include "gridlayout.h"
#include "persistent.h"
#include "stb_ds.h"

bool guiToolButtonGroup_(Painter* p, int* enumv, const char*const fileNames[], int enumSize)
{
    Point pos = getPos();

    if(!guiSameWindow(p, true)) return false;
    Grid* g;
    static struct {
        void* key;
        Grid* value;
    } *map = NULL;
    int index = (int)hmgeti(map, enumv);
    if(index == -1) {
        g = malloc(sizeof(Grid));
        *g = allocateGrid(enumSize, 1, 1);
        g->gridStart = pos;
        hmput(map, enumv, g);
    } else {
        g = map[index].value;
    }
    g->gridStart = pos;
    pushLayout(g);

    bool res = false;
    setCurrentGridPos(0,0);

    for(int i = 0; i < enumSize; i++) {
        if(guiToolButtonEx(p, fileNames[i], true, i == *enumv, NULL, 2)) {
            *enumv = i;
            res = true;
            guiRedrawFromOtherThread(p->window);
        }
        gridNextColumn();
    }

    Size size = {getGridWidth(g),
                 getGridHeight(g)};

    popLayout();
    feedbackSize(size);
    return res;
}

bool guiRadioButtonGroup_(Painter* p, int* enumv, const char*const names[], int enumSize)
{
    Point pos = getPos();

    if(!guiSameWindow(p, true)) return false;
    Grid* g;
    static struct {
        void* key;
        Grid* value;
    } *map = NULL;
    int index = (int)hmgeti(map, enumv);
    if(index == -1) {
        g = malloc(sizeof(Grid));
        *g = allocateGrid(2, enumSize, 1);
        g->gridStart = pos;
        hmput(map, enumv, g);
    } else {
        g = map[index].value;
    }
    g->gridStart = pos;
    pushLayout(g);

    bool res = false;

    Size buttonSize = {15, 15};
    for(int i = 0; i < enumSize; i++) {
        setCurrentGridPos(i,0);
        if(guiToolButtonEx(p, i == *enumv?
                                GUI_RESOURCE_PATH "/radiobuttonOn.png":
                                GUI_RESOURCE_PATH "/radiobuttonOff.png", false, false, &buttonSize, 0)) {
            *enumv = i;
            res = true;
            guiRedrawFromOtherThread(p->window);
        }
        setCurrentGridPos(i,1);
        guiLabelZT(p, names[i]);
    }

    Size size = {getGridWidth(g),
                 getGridHeight(g)};

    popLayout();
    feedbackSize(size);
    return res;
}
