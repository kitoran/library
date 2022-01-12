#ifndef GUI_H
#define GUI_H
#include <X11/Xlib.h>
//#include <assert.h>

#include <stdbool.h>
//enum mode {

//};
//typedef struct State {
//    bool press;
//    int x;
//    int y;
//} State;
extern XEvent xEvent;
//extern State state;
extern Display * xdisplay;
extern XFontStruct *xFontStruct;
extern Window rootWindow;
extern int xDepth;
extern bool timeout;
struct XY {
    int x;
    int y;
};
typedef struct XY Point;
typedef struct XY Size;
extern Point (*getPos)();
extern void (*feedbackSize)(Size);

typedef struct Painter {
    Window window;
    GC gc;
} Painter;

void guiLabel(Painter* p, char *text, int len);
bool guiToolButton(Painter* p, XImage i);
int guiComboBoxZT(Painter* p, char** elements, int current);
void guiNumberEdit(Painter*p, int digits, int* number);

typedef struct GuiWindow {
    Window window;
} GuiWindow ;
void guiDrawLine(Painter*, int, int, int, int);
void guiDrawRectangle(Painter*, int, int, int, int);
void guiSetForeground(Painter*, unsigned long);
void guiDrawTextWithLen(Painter*, int, int, char*, unsigned long);
void guiSetSize(GuiWindow*, uint, uint);


static const unsigned int GuiDarkMagenta = 0x880088;
static inline unsigned int rgb(int r, int g, int b) {
    return r << 16 | g << 8 | b;
}
static inline unsigned int rgbf(double r, double g, double b) {
    return rgb(r*255, g*255, b*255);
}
void guiStartDrawing();
void XNextEventTimeout(Display *display, XEvent *event, double seconds);
#endif // GUI_H
