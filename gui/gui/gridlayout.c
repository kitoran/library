#include "gridlayout.h"
#include "misc.h"
#include <stdio.h>
//#include <ASSERT.h>
#include <stdlib.h>
#include <string.h>

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

Size getGridSize(Grid *g)
{
    return (Size){getGridWidth(g), getGridHeight(g)};
}



Point gridGetPos(Grid* g) {
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

void gridFeedbackSize(Grid* g, Size s) {
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

static const LayoutVT gridVT = {
    /*(Point (*)(struct LayoutVT*))*/gridGetPos,
    (void (*)(struct LayoutVT**,Size))gridFeedbackSize,
    NULL
};

Grid allocateGrid(int cols, int rows, int spacing) {
    Grid r = {.vt = &gridVT, .gridWidths = malloc(4*cols), .gridWidthsLen = cols,
              .gridHeights = malloc(4*rows), .gridHeightsLen = rows,
              .currentX = 0, .currentY = 0, .gridStart = {0,0}, .spacing=spacing};
    memset(r.gridWidths, 0, 4*cols);
    memset(r.gridHeights, 0, 4*rows);
    return r;
}
void clearGrid(Grid* g){
    memset(g->gridWidths, 0, 4*g->gridWidthsLen);
    memset(g->gridHeights, 0, 4*g->gridHeightsLen);
    g->currentX = 0;
    g->currentY = 0;
}
