#include "gridlayout.h"
int gridWidths[200] = {};
int gridHeights[200] = {};
int currentX = 0;
int currentY = 0;
Point gridStart ={0,0};

Point gridGetPos() {
    int x = gridStart.x, y = gridStart.y;
    for(int i = 0; i < currentX; i++) {
        x += gridWidths[i];
    }
    for(int i = 0; i < currentY; i++) {
        y += gridHeights[i];
    }
    Point res = {x, y};
    return res;
}

void gridFeedbackSize(Size s) {
    if(gridWidths[currentX] < s.x) {
        gridWidths[currentX] = s.x;
    }
    if(gridHeights[currentY] < s.y) {
        gridHeights[currentY] = s.y;
    }
}

void setCurrentGridPos(int x, int y) {
    currentX = x;
    currentY = y;
}
