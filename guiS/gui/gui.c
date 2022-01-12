#include "gui.h"
#include <X11/Xlib.h>
#include <assert.h>
#include <stdio.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
Display * xdisplay = 0;
//XFontStruct *xFontStruct;
XFontSet xFontSet;
Window rootWindow;
int xDepth;
//struct Mapping {
//    Window window;
//    void* widget;
//};
//Mapping* mapping = 0;
//void drawVerticalLine(GuiImage *i, int x, int y1, int y2, unsigned int color) {
//    for(int y = MIN(y1,y2); y <MAX(y1,y2); y++) {
//        ((uint*)(i->data))[y*i->width + x] = color;
//    }
//}
//void drawHorizontalLine(GuiImage *i, int x1, int x2, int y, unsigned int color) {
//    for(int x = MIN(x1,x2); x <MAX(x1,x2); x++) {
//        ((uint*)(i->data))[y*i->width + x] = color;
//    }
//}

void guiDrawLine(Painter *a, int b, int c, int d, int e)
{
    abort();
}

void guiDrawRectangle(Painter *a, int b, int c, int d, int e)
{
    abort();
}

void guiSetForeground(Painter *a, unsigned long b)
{
    abort();
}

void guiDrawTextWithLen(Painter *a, int b, int c, char *d, unsigned long e)
{
    abort();
}

void guiSetSize(GuiWindow *win, uint w, uint h)
{
    XResizeWindow(xdisplay, win->window, w, h);
}

void guiLabel(Layout* l, char *text, int len)
{
    XRectangle overallInk;
    XRectangle overallLog;

    Xutf8TextExtents(xFontSet, text, len, &overallInk, &overallLog);
    int width = overallLog.width + 10;
    int height = overallLog.height + 10;

//    XDrawRect
//    Window label = XCreateSimpleWindow(xdisplay, w->window, x, y, *width, *height, 0, 0, 0x00);
//    XSelectInput(xdisplay, label, ExposureMask);
//    XMapWindow(xdisplay, label);
//    GC gc = XCreateGC(xdisplay , label, 0, NULL);
//    XTextItem ti = {
//        text,
//        len,
//        0,
//        None
//    };
    XSetForeground(xdisplay, l->gc, 0xffffffff);
    XDrawString(xdisplay, l->window, l->gc,
                l->x+5 + overallLog.x, l->y+5 - overallLog.y, text, len);
    l->x += width+5;
    l->maxHeight = MAX(l->maxHeight, height);
    //    XFlush(xdisplay);
//    GuiLabel res = {label};
//    return res;
}

bool guiButton(Layout *l, char* text, int len)
{
    XRectangle overallInk;
    XRectangle overallLog;

    Xutf8TextExtents(xFontSet, text, len, &overallInk, &overallLog);
    int width = overallLog.width + 10;
    int height = overallLog.height + 10;
    if(xEvent.type == Expose) {
        XSetForeground(xdisplay, l->gc, 0xff555555);
//        XSetBackground(xdisplay, l->gc, 0xffff5555);
        XFillRectangle(xdisplay, l->window, l->gc, l->x, l->y, width, height);
        XSetForeground(xdisplay, l->gc, 0xffffffff);
        Xutf8DrawString(xdisplay, l->window, xFontSet, l->gc,
                    l->x+5 + overallLog.x, l->y+5 - overallLog.y, text, len);
    }
//    XPutImage(xdisplay, l->window, l->gc, l->x+5, l->y+5, width, height);
    bool res = false;
    if(xEvent.type == ButtonPress) {
        int x = xEvent.xbutton.x;
        int y = xEvent.xbutton.y;
        if(x >= l->x && x <= l->x +width &&
            y >= l->y && y <= l->y +height) {
            res = true;
        }
    }
    l->x += width+5;
    l->maxHeight = MAX(l->maxHeight, height);
    return res;
}

void guiStartDrawing() {

    xdisplay = XOpenDisplay(NULL);
    assert(xdisplay);

    int numberoffonts;
    char** fonts  = XListFonts(xdisplay,
                            "*",
                               1000,
                               &numberoffonts);
    char **missingList;
    int missingCount;
    char *defString;
    xFontSet =
        XCreateFontSet(xdisplay,  "-*-*-*-*-*-*-*-*-*-*-*-*-*-*",
                       &missingList, &missingCount, &defString);
    if (xFontSet == NULL) {
        fprintf(stderr, "Failed to create fontset\n");
        abort();
    }
    XFreeStringList(missingList);

    int screen = DefaultScreen(xdisplay);
    int blackColor = BlackPixel(xdisplay, screen );
    int whiteColor = WhitePixel(xdisplay, screen );
    xDepth = DefaultDepth(xdisplay, screen);

    printf("Depth %d\n", xDepth);
    // Create the window
    rootWindow = XCreateSimpleWindow(xdisplay , DefaultRootWindow(xdisplay ), 0, 0,
                                        600, 300, 0, blackColor, blackColor);

    // We want to get MapNotify events
    XSelectInput(xdisplay, rootWindow, StructureNotifyMask | ButtonPressMask
                 | ExposureMask);

    XStoreName(xdisplay, rootWindow, "Hello, World!");
    // "Map" the window (that is, make it appear on the screen)
    XMapWindow(xdisplay , rootWindow);

    // Create a "Graphics Context"
    GC gc = XCreateGC(xdisplay , rootWindow, 0, NULL);
//    xFontStruct = XLoadQueryFont(xdisplay, "fixed");
    // Tell the GC we draw using the white color
    XSetForeground(xdisplay , gc, whiteColor);
    // Wait for the MapNotify event
    for(;;) {
        XEvent e;
        XNextEvent(xdisplay, &e);
        if (e.type == MapNotify)
            break;
    }

//    // Draw the line
//    XDrawLine(xdisplay, window, gc, 10, 60, 180, 20);
//    int screen = XDefaultScreen(xdisplay);
//    Visual *visual = DefaultVisual(xdisplay, screen);
//    char data[300*300*4];
//    XImage* image = XCreateImage(xdisplay, visual, 24, ZPixmap, 0, data,
//                                 300, 300, 32, 0);
//    //    struct funcs funcs;
//    //    XPointer xp = 0;
//    //    XImage image = {
//    //        300, 300,
//    //        0,
//    //        XYPixmap,
//    //        malloc(300*300*4),
//    //        LSBFirst,
//    //        8, LSBFirst, 8, //for bitmaps
//    //        24/*8*/, // (8 or 24?) depth
//    //        300*4, //bytes_per_line
//    //        32, //for zpixmap
//    //        0x00ff0000,//        unsigned long red_mask;    /* bits in z arrangement */
//    //        0x0000ff00, //unsigned long green_mask;
//    //        0x000000ff, //unsigned long blue_mask;
//    //        &image,
//    //        funcs,
//    //    };
//    XInitImage(image);
//    for(int i = 0; i < 300*300*4; i++) {
//        image->data[i]=i%253;
//    }

//    XPutImage(xdisplay, window, gc, image, 0, 0, 0, 0, 300, 300);


//    // Send the "DrawLine" request to the server
    XFlush(xdisplay);

}

