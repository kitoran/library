#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H
#include "gui.h"
#include "layoutStack.h"

struct Grid;
typedef struct Grid {
    LayoutVT* vt;
    int * gridWidths;
    int gridWidthsLen;
    int * gridHeights;
    int gridHeightsLen;
    int spacing;
    int currentX;
    int currentY;
    Point gridStart;
} Grid;

Grid allocateGrid(int cols, int rows, int spacing);
void clearGrid(Grid* g);
void setCurrentGridPos(int row, int column);
void gridNextRow();
void gridNextColumn();
int getGridHeight(Grid *g);
inline int getGridBottom(Grid *g) {
//    DEBUG_PRINT(g->gridStart.y + getGridHeight(g), "%d");
  return g->gridStart.y + getGridHeight(g);
}
int getGridWidth(Grid *g);
Size getGridSize(Grid *g);


#endif // GRIDLAYOUT_H
