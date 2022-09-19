#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H
// For now this is the single grid layout
// if i ever need different layouts in one gui i will change this
#include "gui.h"

struct Grid;
typedef struct Grid {
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
void pushGrid(Grid*);
void popGrid();
Grid* topGrid();
Point gridGetPos();
void gridFeedbackSize(Size s);
void setCurrentGridPos(Grid *g, int row, int column);
void gridNextRow(Grid *g);
void gridNextColumn(Grid *g);
int getGridHeight(Grid *g);
inline int getGridBottom(Grid *g) {
  return g->gridStart.y + getGridHeight(g);
}
int getGridWidth(Grid *g);


#endif // GRIDLAYOUT_H
