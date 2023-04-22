#ifndef LINELAYOUT_H
#define LINELAYOUT_H

#include "gui.h"
#include "layoutStack.h"

typedef struct LineLayout {
    const LayoutVT* vt;
    bool horizontal;
    int filled;
    int across;
    int spacing;
    Point pos;
} LineLayout;

LineLayout makeHorizontalLayout(int spacing);
LineLayout makeVerticalLayout(int spacing);
Size getLineSize(LineLayout *ll);
#endif // LINELAYOUT_H
