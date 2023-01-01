#include "linelayout.h"
#include "misc.h"
#include <stdio.h>
//#include <ASSERT.h>
#include <stdlib.h>
#include <string.h>

/*Grid allocateGrid(int cols, int rows, int spacing) {
    Grid r = {.gridWidths = malloc(4*cols), .gridWidthsLen = cols,
              .gridHeights = malloc(4*rows), .gridHeightsLen = rows,
              .currentX = 0, .currentY = 0, .gridStart = {0,0}, .spacing=spacing};
    memset(r.gridWidths, 0, 4*cols);
    memset(r.gridHeights, 0, 4*rows);
    return r;
}
//fprintf(stderr, msg, ##__VA_ARGS__);
void setCurrentGridPos(int row, int column) {
    Grid* g = topLayout();
    ASSERT(row < g->gridHeightsLen, "row too big")
    ASSERT(column < g->gridWidthsLen, "col too big")
    g->currentX = column;
    g->currentY = row;
}
int getGridHeight(Grid* g)
{
    int y = 0;
    for(int i = 0; i < g->gridHeightsLen; i++) {
        if(g->gridHeights[i])
            y += g->gridHeights[i]+g->spacing;
    }
    return y+g->spacing;
}

int getGridWidth(Grid *g)
{
    int x = 0;
    for(int i = 0; i < g->gridWidthsLen; i++) {
        if(g->gridWidths[i])
            x += g->gridWidths[i]+g->spacing;
    }
    return x+g->spacing;
}

int getGridBottom(Grid *g);
void gridNextRow()
{
    Grid* g = topLayout();
//    ASSERT(g->gridWidthsLenvsdv
    g->currentY++;
}

void gridNextColumn()
{
    Grid* g = topLayout();
    g->currentY = 0;
    g->currentX++;
}

Grid* stack[10];
size_t stackTop = 0;

void pushLayout(Grid *g)
{
    ASSERT(stackTop < ELEMS(stack), "pushing grid, stackTop is %lud, elems is %lud, "
           "cond is %d, !cond id %d", stackTop, ELEMS(stack),
           stackTop < ELEMS(stack), !(stackTop < ELEMS(stack)))
    stack[stackTop] = g;
    stackTop++;
}

void popLayout()
{
    ASSERT(stackTop > 0, "stack is empty, trying to pop")
    stack[stackTop] = NULL;
    stackTop--;
}

Point gridGetPos() {
//    ASSERT(stackTop > 0, "stack is empty, trying to use")
    Grid* g = topLayout();
    ASSERT(g->currentX < g->gridWidthsLen, "%d, %d", g->currentX, g->gridWidthsLen)
    ASSERT(g->currentY < g->gridHeightsLen,"")
    int x = g->gridStart.x, y = g->gridStart.y;
    for(int i = 0; i < g->currentX; i++) {
        if(g->gridWidths[i])
            x += g->gridWidths[i]+g->spacing;
    }
    for(int i = 0; i < g->currentY; i++) {
        if(g->gridHeights[i])
            y += g->gridHeights[i]+g->spacing;
    }
    Point res = {x, y};
    return res;
}

void gridFeedbackSize(Size s) {
    ASSERT(stackTop > 0, "stack is empty, gridFeedbackSize")
    Grid* g = stack[stackTop-1];
    ASSERT(g->currentX < g->gridWidthsLen, "%d, %d", g->currentX, g->gridWidthsLen)
    ASSERT(g->currentY < g->gridHeightsLen,"")
//            if(s.h > 50 && g->currentY < 3) {
//                ASSERT(false, "");
//            }
    Grid* fw3ehbsergser = g;
    if(s.h > 1000) {
        fprintf(stderr, "deep %p", fw3ehbsergser);
    }
    if(g->gridWidths[g->currentX] < (int)s.w) {
        guiRedraw();
        g->gridWidths[g->currentX] = (int)s.w;
    }
    if(g->gridHeights[g->currentY] < (int)s.h) {
        guiRedraw();
        g->gridHeights[g->currentY] = (int)s.h;
    }
}

Grid *topLayout()
{
    ASSERT(stackTop > 0, "stack is empty, trying to get top")
    return stack[stackTop-1];
}

Size getGridSize(Grid *g)
{
    return (Size){getGridWidth(g), getGridHeight(g)};
}
*/
