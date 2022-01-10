#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds

#define NIL (0)       // A name for the void pointer
#include <stdio.h>

#include <stdbool.h>
#include "gui.h"

int main()
{

    Display *display = XOpenDisplay(NIL);
    assert(display);

    int screen = DefaultScreen(display);
    int blackColor = BlackPixel(display, screen );
    int whiteColor = WhitePixel(display, screen );
    int depth = DefaultDepth(display, screen);
    printf("%d\n", depth);
    int*r, c;
    r = XListDepths(display, screen, &c);
//    return 0;
    // Create the window
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0,
                300, 300, 0, blackColor, blackColor);

    // We want to get MapNotify events
    XSelectInput(display, window, StructureNotifyMask | ButtonPressMask);

    XStoreName(display, window, "Hello, World!");
    // "Map" the window (that is, make it appear on the screen)
    XMapWindow(display, window);


    // Wait for the MapNotify event
    for(;;) {
      XEvent e;
      XNextEvent(display, &e);
      if (e.type == MapNotify)
        break;
    }
    Pixmap p = XCreatePixmap(display, window, 300, 200, depth );
    // Create a "Graphics Context"
    GC gc = XCreateGC(display, p, 0, NIL);
    GC gc2 = XCreateGC(display, window, 0, NIL);
    // Tell the GC we draw using the white color
    XSetForeground(display, gc, 0xff040404);
    XFillRectangle(display, p, gc, 0,0,300,200);
    XSetForeground(display, gc, whiteColor);
    XDrawLine(display, p, gc, 10, 60, 180, 20);

    // Draw the line

    Visual *visual = DefaultVisual(display, screen); 
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
    XDrawLine(display,p,gc,0,10,290,180);
//    drawVerticalLine(&rr, 50, 10, 150, 0x00ff0000);
//    drawHorizontalLine(&rr, 50, 150, 50, 0x0000ff00);
    XCopyArea(display, p, window, gc2, 0,0, 300, 200, 0,0);
//    XImage* image = XCreateImage(display, visual, 24, ZPixmap, 0, data,
//                                 300, 200, 32, 0);
    XImage* image = XGetImage(display, p, 0,0,
                                 300, 200,  ~0,ZPixmap);
//    XInitImage(image);
//    XDrawRectangle(display, (Drawable)image, gc, 20, 20, 100, 100);


//    XPutImage(display, window, gc, image, 0, 0, 0, 0, 300, 300);


    // Send the "DrawLine" request to the server
    XFlush(display);

    while(true) {
        XEvent e;
        XNextEvent(display, &e);
        switch(e.type)
        {
        case DestroyNotify: {
            return 0;
        } break;
        case ButtonPress: {
            XPutImage(display, window, gc, image, 0, 0, 0, 0, 300, 200);
        } break;
        }
    }
    XCloseDisplay(display);
    return 0;
}
