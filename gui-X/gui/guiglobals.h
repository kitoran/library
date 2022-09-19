#include "gui.h"
extern Display * xdisplay;
//XFontStruct *xFontStruct;
extern XFontSet xFontSet;
extern Window rootWindow;
extern int xDepth;
extern int maxDigitWidth;
extern int maxDigitHeight;
extern bool redraw;
struct context {
    void* active;
    int pos;
};
extern struct context context;
extern Size rootWindowSize;
