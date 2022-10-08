#include "gui.h"
#include "guiglobals.h"
Display * xdisplay = 0;
//XFontStruct *xFontStruct;
//XEvent xEvent;
XFontSet xFontSet;
Window rootWindow = 0;
int xDepth = 0;
int maxDigitWidth = 0;
int maxDigitHeight = 0;
struct _XftFont  *xFont;
bool redraw = false;
struct context context = {
    NULL,
    0,
    {0},
    0,
    0
};
Size rootWindowSize;
