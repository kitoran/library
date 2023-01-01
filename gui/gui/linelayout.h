#ifndef LINELAYOUT_H
#define LINELAYOUT_H
/*#include "gui.h"

typedef struct LineLayout {
    int * sizesAlong;
    int sizesAlongLen;
    int sizeAcross;
    int spacing;
    int currentCell;
    Point start;
} LineLayout;

LineLayout allocateLineLayout(int cols, int rows, int spacing);
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
//    DEBUG_PRINT(g->gridStart.y + getGridHeight(g), "%d");
  return g->gridStart.y + getGridHeight(g);
}
int getGridWidth(Grid *g);
Size getGridSize(Grid *g);
*/
#endif // LINELAYOUT_H
