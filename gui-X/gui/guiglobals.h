#include "gui.h"
extern Display * xdisplay;
//XFontStruct *xFontStruct;
extern XFontSet xFontSet;
struct XftFont;
extern struct XftFont *xFont;
extern Window rootWindow;
extern int xDepth;
extern int maxDigitWidth;
extern int maxDigitHeight;
extern bool redraw;
struct context {
    void* active;
    int pos;
#define MAX_DIGITS 30
    char editedString[MAX_DIGITS];
    char terminator;
    int editedStringLen;
    void (*commit)(void* data);
};
extern struct context context;
extern Size rootWindowSize;
