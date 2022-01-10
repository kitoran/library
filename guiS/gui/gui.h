#ifndef GUI_H
#define GUI_H
#include <X11/Xlib.h>
#include <assert.h>
//enum mode {

//};
extern Display * xdisplay;
extern XFontStruct *xFontStruct;
extern Window rootWindow;
extern int xDepth;

typedef struct Painter {

} Painter;

typedef struct Layout {
    int x;
    int y;
    int maxHeight;
    Window window;
    GC gc;
} Layout;

//Layout createLayout() {
//    Layout res = {5, 5, 0};
//    return res;
//}

void guiLabel(Layout* l, char *text, int len);
//
typedef struct GuiLabel {
    Window window;

} GuiLabel;
typedef struct GuiToolButton {


} GuiToolButton;
typedef struct GuiComboBox {


} GuiComboBox;
typedef struct GuiEntry {


} GuiEntry;
typedef struct GuiSpinButton {


} GuiSpinButton;

typedef struct GuiButton {


} GuiButton ;
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

#endif // GUI_H
