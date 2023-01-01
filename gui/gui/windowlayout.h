#ifndef WINDOWLAYOUT_H
#define WINDOWLAYOUT_H
// For now this is the single window layout
// if i ever need different window layouts in one gui i will change this
#include "gui.h"

/*********************
 * Layout looks like this:
 *  File | Edit | View | Window | Help          <-- menu bar
 *  play | stop | pencil | eraser | scale       <-- toolbar (just 1 row, if i ever need to changethis i will)
 *  /***********************************\
 *  *          main area                *
 *  *                                   *
 *  \********************************** /
 *  bpm | grid | snap                           <-- bottom toolbar (in the future maybe)
 *  3s | 4 measures | 330 hz | vel 120          <-- status bar (in the future maybe)


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
void pushLayout(Grid*);
void popLayout();
Grid* topLayout();
Point gridGetPos();
void gridFeedbackSize(Size s);
void setCurrentGridPos(int row, int column);
void gridNextRow();
void gridNextColumn();
int getGridHeight(Grid *g);
inline int getGridBottom(Grid *g) {
  return g->gridStart.y + getGridHeight(g);
}
int getGridWidth(Grid *g);


*/
#endif // WINDOWLAYOUT_H
