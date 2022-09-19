#include "gridlayout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Grid allocateGrid(int cols, int rows, int spacing) {
    Grid r = {.gridWidths = malloc(4*cols), .gridWidthsLen = cols,
              .gridHeights = malloc(4*rows), .gridHeightsLen = rows,
              .currentX = 0, .currentY = 0, .gridStart = {0,0}, .spacing=spacing};
    memset(r.gridWidths, 0, 4*cols);
    memset(r.gridHeights, 0, 4*rows);
    return r;
}

void setCurrentGridPos(Grid* g, int row, int column) {
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
    return y;
}

int getGridWidth(Grid *g)
{
    int x = 0;
    for(int i = 0; i < g->gridWidthsLen; i++) {
        if(g->gridWidths[i])
            x += g->gridWidths[i]+g->spacing;
    }
    return x;
}

int getGridBottom(Grid *g);
void gridNextRow(Grid* g)
{
    g->currentY++;
}

void gridNextColumn(Grid* g)
{
    g->currentY = 0;
    g->currentX++;
}


Grid* stack[10];
int stackTop = -1;

void pushGrid(Grid *g)
{
    stackTop++;
    stack[stackTop] = g;
}

void popGrid()
{
    stackTop--;
}

Point gridGetPos() {
    Grid* g = stack[stackTop];
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
    Grid* g = stack[stackTop];
    Grid* fw3ehbsergser = g;
    if(s.height > 1000) {
        fprintf(stderr, "deep %p", fw3ehbsergser);
    }
    if(g->gridWidths[g->currentX] < (int)s.width) {
        guiRedraw();
        g->gridWidths[g->currentX] = (int)s.width;
    }
    if(g->gridHeights[g->currentY] < (int)s.height) {
        guiRedraw();
        g->gridHeights[g->currentY] = (int)s.height;
    }
}

Grid *topGrid()
{
    return stack[stackTop];
}
