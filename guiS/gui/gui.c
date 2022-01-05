#include "gui.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
Display * xdisplay = 0;

void drawVerticalLine(GuiImage *i, int x, int y1, int y2, unsigned int color) {
    for(int y = MIN(y1,y2); y <MAX(y1,y2); y++) {
        ((uint*)(i->data))[y*i->width + x] = color;
    }
}
void drawHorizontalLine(GuiImage *i, int x1, int x2, int y, unsigned int color) {
    for(int x = MIN(x1,x2); x <MAX(x1,x2); x++) {
        ((uint*)(i->data))[y*i->width + x] = color;
    }
}
