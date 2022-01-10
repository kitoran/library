#ifndef GUI_H
#define GUI_H
#include <X11/Xlib.h>
#include <assert.h>
//enum mode {

//};


extern Display * xdisplay;
static const unsigned int GuiDarkMagenta = 0x880088;
inline unsigned int rgb(int r, int g, int b) {
    return r << 16 | g << 8 | b;
}
inline unsigned int rgbf(double r, double g, double b) {
    return rgb(r*255, g*255, b*255);
}



static void startDrawing() {

    xdisplay = XOpenDisplay(NULL);
    assert(xdisplay);

    int blackColor = BlackPixel(xdisplay , DefaultScreen(xdisplay));
    int whiteColor = WhitePixel(xdisplay , DefaultScreen(xdisplay));

    // Create the window
    Window window = XCreateSimpleWindow(xdisplay , DefaultRootWindow(xdisplay ), 0, 0,
                300, 300, 0, blackColor, blackColor);

    // We want to get MapNotify events
    XSelectInput(xdisplay , window, StructureNotifyMask);

    XStoreName(xdisplay, window, "Hello, World!");
    // "Map" the window (that is, make it appear on the screen)
    XMapWindow(xdisplay , window);

    // Create a "Graphics Context"
    GC gc = XCreateGC(xdisplay , window, 0, NULL);

    // Tell the GC we draw using the white color
    XSetForeground(xdisplay , gc, whiteColor);
    // Wait for the MapNotify event
    for(;;) {
      XEvent e;
      XNextEvent(xdisplay, &e);
      if (e.type == MapNotify)
        break;
    }

    // Draw the line
    XDrawLine(xdisplay, window, gc, 10, 60, 180, 20);
    int screen = XDefaultScreen(xdisplay);
    Visual *visual = DefaultVisual(xdisplay, screen);
    char data[300*300*4];
    XImage* image = XCreateImage(xdisplay, visual, 24, ZPixmap, 0, data,
                                 300, 300, 32, 0);
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
    XInitImage(image);
    for(int i = 0; i < 300*300*4; i++) {
        image->data[i]=i%253;
    }

    XPutImage(xdisplay, window, gc, image, 0, 0, 0, 0, 300, 300);


    // Send the "DrawLine" request to the server
    XFlush(xdisplay);

}

#endif // GUI_H
