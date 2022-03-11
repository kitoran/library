#ifndef GUI_H
#define GUI_H
#include <X11/Xlib.h>
//#include <assert.h>

#include <stdbool.h>

#define TimerEvent LASTEvent+1

//enum mode {

//};
//typedef struct State {
//    bool press;
//    int x;
//    int y;
//} State;
typedef struct GuiImage {

} GuiImage;
extern XEvent xEvent;
//extern State state;
extern Display * xdisplay;
extern XFontStruct *xFontStruct;
extern Window rootWindow;
extern int xDepth;

typedef struct Point {
    int x;
    int y;
} Point;
typedef struct Size {
    uint width;
    uint height;
} Size;
extern Point (*getPos)();
extern void (*feedbackSize)(Size);


typedef struct Painter {
    Drawable drawable;
    GC gc;
} Painter;
typedef struct RawPicture {
    char* data;
    size_t w;
    size_t h;
} RawPicture ;

void guiLabel(Painter* p, char *text, int len);
int guiComboBoxZT(Painter* p, char** elements, int current);
bool guiNumberEdit(Painter*p, int digits, int* number, bool* consume);
void guiDoubleEdit(Painter*p, int digits, double* number);
bool guiButton(Painter *p, char* text, int len);
bool guiToolButton(Painter* p, XImage* i, bool *consume);
void guiLabelZT(Painter* p, char *text);
bool guiButtonZT(Painter* p, char *text);

void guiDrawLine(Painter*, int, int, int, int);
void guiDrawRectangle(Painter*, int, int, int, int);
void guiFillRectangle(Painter *a, int b, int c, int d, int e);
void guiFillRawRectangle(RawPicture *p, int x, int y, int w, int h, char r, char g, char b);
void guiSetForeground(Painter*, unsigned long);
void guiDrawTextWithLen(Painter*, int, int, char*, unsigned long);
void guiSetSize(Window, uint, uint);
Size guiGetSize();


static const unsigned int GuiDarkMagenta = 0x880088;
static inline unsigned int rgb(int r, int g, int b) {
    return r << 16 | g << 8 | b;
}
static inline unsigned int rgbf(double r, double g, double b) {
    return rgb(r*255, g*255, b*255);
}
void guiStartDrawing();
void guiNextEvent();
void guiRedraw();
#endif // GUI_H
