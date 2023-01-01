#include "toolbuttongroup.h"
#include "stdbool.h"
#include "gui.h"
#include "gridlayout.h"
#include "persistent.h"
#include "stb_ds.h"

bool guiToolButtonGroup_(Painter* p, int* enumv, const char*const fileNames[], int enumSize)
{
    Point pos = getPos();

    if(!guiSameWindow(p)) return false;
    Grid* g;
    static struct {
        void* key;
        Grid* value;
    } *map = NULL;
    int index = hmgeti(map, enumv);
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
        if(resourseToolButtonEx(p, fileNames[i], i == *enumv, NULL)) {
            *enumv = i;
            res = true;
            guiRedraw();
        }
        gridNextColumn();
    }

    Size size = {getGridWidth(g),
                 getGridHeight(g)};

    popLayout();
    feedbackSize(size);
    return res;
}
