#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds

#define NIL (0)       // A name for the void pointer
#include <stdio.h>

#include <stdbool.h>
#include <locale.h>
#include "gui.h"
#include "gridlayout.h"
//#include <X11/Xft/Xft.h>
//XftFont* font;
//XftDraw* xftDraw;
int fe;
void loop(Painter* p) {
//
//    XFlush(xdisplay);
    setCurrentGridPos(0,0);
    guiLabel(p, "hello", 5);
    setCurrentGridPos(0,1);
    guiLabel(p, "yes", 3);
    setCurrentGridPos(1,0);
    guiLabel(p, "i", 1);
    setCurrentGridPos(1,1);
    guiLabel(p, "am", 2);
    setCurrentGridPos(0,2);
    guiLabel(p, "an", 2);
    setCurrentGridPos(1,2);
    guiLabel(p, "label", 5);

    setCurrentGridPos(2,2);
    if(guiButton(p, "button", 6)) {
        fprintf(stderr, "button was pressed");
    }

    setCurrentGridPos(2,1);

    char hi[] = "привет→";
    if(guiButton(p, hi, sizeof(hi)-1)) {
        fprintf(stderr, "button2 was pressed");
    }
    setCurrentGridPos(2,0);
    guiNumberEdit(p, 9, &fe);
}


XEvent xEvent;
int main()
{

    char* l = setlocale(LC_ALL, "C.UTF-8");
    l = setlocale(LC_ALL, "C.UTF-8");
    guiStartDrawing();
//    font = XftFontOpen (xdisplay, XDefaultScreen(xdisplay),
//                            XFT_FAMILY, XftTypeString, "FreeSerif",
//                            XFT_SIZE, XftTypeDouble, 12.0,
//                            NULL);
    int s = DefaultScreen(xdisplay);
//    xftDraw = XftDrawCreate(xdisplay, rootWindow,
//                            XDefaultVisual(xdisplay, s),
//                            DefaultColormap(xdisplay, s));

    getPos = gridGetPos;
    feedbackSize = gridFeedbackSize;
    gridStart.x = 5;
    gridStart.y = 5;

    GC gc2 = XCreateGC(xdisplay, rootWindow, 0, NIL);
    Painter pa = {rootWindow, gc2};
    loop(&pa);
    XClearWindow(xdisplay, rootWindow);
    Pixmap p = XCreatePixmap(xdisplay, rootWindow, 300, 200, xDepth );
    // Create a "Graphics Context"
    GC gc = XCreateGC(xdisplay, p, 0, NIL);
    // Tell the GC we draw using the white color
    XSetForeground(xdisplay, gc, 0xffa4a4a4);
    XFillRectangle(xdisplay, p, gc, 0,0,300,200);
    XSetForeground(xdisplay, gc, 0xffffffff);
    XDrawLine(xdisplay, p, gc, 10, 60, 180, 20);
    XTextItem ti = {
        "hello i am a text",
        strlen(ti.chars),
        0,
        None
    };
//    XDrawText(xdisplay, p, gc, 30, 30, &ti, 1);
    // Draw the line

//    Visual *visual = DefaultVisual(display, Defascreen);
    char data[300*200*4];
//    struct funcs funcs;
//    XPointer xp = 0;
//    XImage image = {
//        300, 300,
//        0,
//        XYPixmap,
//        malloc(300*300*4),
//        LSBFirst,
//        8, LSBFirst, 8, //for bitmaps
//        24/*8*/, // (8 or 24?) depth
//        300*4, //bytes_per_line
//        32, //for zpixmap
//        0x00ff0000,//        unsigned long red_mask;    /* bits in z arrangement */
//        0x0000ff00, //unsigned long green_mask;
//        0x000000ff, //unsigned long blue_mask;
//        &image,
//        funcs,
//    };
    
//    GuiImage rr = {300, 200, data};
    for(int i = 0; i < 300*200*4; i++) {
        data[i]=i%253;
    }
    XDrawLine(xdisplay,p,gc,0,10,290,180);
//    drawVerticalLine(&rr, 50, 10, 150, 0x00ff0000);
//    drawHorizontalLine(&rr, 50, 150, 50, 0x0000ff00);
    XCopyArea(xdisplay, p, rootWindow, gc2, 0,0, 300, 200, 0,0);
//    XImage* image = XCreateImage(display, visual, 24, ZPixmap, 0, data,
//                                 300, 200, 32, 0);
    XImage* image = XGetImage(xdisplay, p, 0,0,
                                 300, 200,  ~0,ZPixmap);
//    XInitImage(image);
//    XDrawRectangle(display, (Drawable)image, gc, 20, 20, 100, 100);

    GuiWindow www = {rootWindow};
    int xx = 10, yy = 10;
    int ww, hh, hhmax;
//    guiCreateLabelTextWithLen(&www, xx, yy, "hello", 5, &ww, &hh);
//    xx += ww + 10;
//    guiCreateLabelTextWithLen(&www, xx, yy, "yes", 3, &ww, &hh);
//    xx += ww + 10;
//    guiCreateLabelTextWithLen(&www, xx, yy, "i", 1, &ww, &hh);
//    xx += ww + 10;
//    guiCreateLabelTextWithLen(&www, xx, yy, "am", 2, &ww, &hh);
//    xx += ww + 10;
//    guiCreateLabelTextWithLen(&www, xx, yy, "an", 2, &ww, &hh);
//    xx += ww + 10;
//    GuiLabel l = guiCreateLabelTextWithLen(&www, xx, yy, "label", 5, &ww, &hh);


//    XPutImage(display, window, gc, image, 0, 0, 0, 0, 300, 300);

    // Send the "DrawLine" request to the server
    XFlush(xdisplay);
//    loop(&pa);
//    XNextEvent(xdisplay, &xEvent);
    while(true) {
        XNextEventTimeout(xdisplay, &xEvent, 0.530);
        loop(&pa);
        timeout = false;
//        XRenderColor r = {16000, 16000, 16000, 16000};
//        XftDrawStringUtf8 (xftDraw,
//                        &r,
//                        font,
//                        100,
//                        100,
//                        hi, sizeof(hi)-1);
        XFlush(xdisplay);
//        XEvent e;
//        switch(e.type)
//        {
        if(xEvent.type == DestroyNotify) {
            goto exit;
        };
//        case Expose: {
//            fprintf(stderr, "expose %d %d %d\n", e.xexpose.window,
//                   www.window, l.window);
////            XPutImage(xdisplay, rootWindow, gc, image, 0, 0, 0, 0, 300, 200);
//        } break;
//        case ButtonPress: {
//            guiSetSize(&www, 400, 500);
////            XPutImage(xdisplay, rootWindow, gc, image, 0, 0, 0, 0, 300, 200);
//        } break;
//        }
    }
    exit:
    XCloseDisplay(xdisplay);
    return 0;
}
