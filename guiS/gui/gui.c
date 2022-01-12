#include "gui.h"
#include <X11/Xlib.h>
#include <assert.h>
#include <stdio.h>
#define XK_LATIN1
#define XK_MISCELLANY
#include <X11/keysymdef.h>
#include <sys/select.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
Display * xdisplay = 0;
//XFontStruct *xFontStruct;
XFontSet xFontSet;
Window rootWindow;
int xDepth;
int maxDigitWidth = 0;
int maxDigitHeight = 0;
bool timeout = false;

struct {


    void* active;
    int pos;
} context;


Point defaultGetPos() {
    fprintf(stderr, "getPos not set");
    abort();
}
void defaultFeedbackSize(Size s) {

}

Point (*getPos)() = defaultGetPos;
void (*feedbackSize)(Size) = defaultFeedbackSize;
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

void guiLabel(Painter* p, char *text, int len)
{
    Point pos = getPos();
    XRectangle overallInk;
    XRectangle overallLog;

    Xutf8TextExtents(xFontSet, text, len, &overallInk, &overallLog);
    Size size = {overallLog.width + 10,
                overallLog.height + 10};

//    XDrawRectangle(xdisplay, p->window, p->gc, pos.x, pos.y,
//                   size.x, size.y);
    if(xEvent.type == Expose) {
        XSetForeground(xdisplay, p->gc, WhitePixel(xdisplay,
                                                   DefaultScreen(xdisplay)));
        fprintf(stderr, "printing label %s %d %d\n",
                text, pos.x+5 + overallLog.x, pos.y+5 - overallLog.y);
        Xutf8DrawString(xdisplay, p->window, xFontSet, p->gc,
                    pos.x+5 + overallLog.x, pos.y+5 - overallLog.y, text, len);
    }
//    p->x += width+5;
//    p->maxHeight = MAX(p->maxHeight, height);
    feedbackSize(size);
    //    XFlush(xdisplay);
//    GuiLabel res = {label};
//    return res;
}

bool guiButton(Painter *p, char* text, int len)
{
    Point pos = getPos();
    XRectangle overallInk;
    XRectangle overallLog;

    Xutf8TextExtents(xFontSet, text, len, &overallInk, &overallLog);
    Size size = {overallLog.width + 10,
                overallLog.height + 10};
    if(xEvent.type == Expose) {
        XSetForeground(xdisplay, p->gc, 0xff555555);
//        XSetBackground(xdisplay, l->gc, 0xffff5555);
        XFillRectangle(xdisplay, p->window, p->gc, pos.x, pos.y,
                       size.x, size.y);
        XSetForeground(xdisplay, p->gc, 0xffffffff);
        Xutf8DrawString(xdisplay, p->window, xFontSet, p->gc,
                    pos.x+5 + overallLog.x, pos.y+5 - overallLog.y, text, len);
    }
//    XPutImage(xdisplay, l->window, l->gc, l->x+5, l->y+5, width, height);
    bool res = false;
    if(xEvent.type == ButtonPress) {
        int mx = xEvent.xbutton.x;
        int my = xEvent.xbutton.y;
        if(mx >= pos.x && mx <= pos.x + size.x &&
            my >= pos.y && my <= pos.y + size.y) {
            res = true;
        }
    }
//    p->x += width+5;
//    p->maxHeight = MAX(p->maxHeight, height);
    feedbackSize(size);
    return res;
}


void guiNumberEdit(Painter *p, int digits, int *number) {
    Point pos = getPos();
    static bool cursor = false;

    Size size = {maxDigitWidth*digits + 10,
                maxDigitHeight + 10};

    int numberOfDigits = 0;
    {
        int curNumber = *number;
        while(curNumber) {
            numberOfDigits++;
            curNumber/=10;
        }
        if(*number <= 0) {
            numberOfDigits++;
        }
    }


    if(xEvent.type == KeyPress && context.active == number) {
        KeySym sym = XLookupKeysym(&xEvent.xkey, 0);
        fprintf(stderr, "%c %c! \n", sym, XK_Right);
        if(sym == XK_Right) {
            if(context.pos < numberOfDigits) {
                context.pos++;
            }
            cursor = true;
        } else if(sym == XK_Left) {
            if(context.pos > 0) {
                context.pos--;
            }
            cursor = true;
        } else if(sym >= '0' && sym <= '9' || sym == XK_BackSpace) {
            if(context.pos == 0 && *number < 0) {
                goto keyPressBreak;
            }
            int valueExponent = numberOfDigits - context.pos;
            int value = 1;
            for(int i = 0; i < valueExponent; i++) {
                value*=10;
            }
            if(sym == XK_BackSpace) {
                if(context.pos == 0) {
                    goto keyPressBreak;
                } else if(context.pos == 1  && *number < 0) {
                    *number = -*number;
                } else {
                    *number = (*number)/value/10*value + (*number)%value;
                }
                if(context.pos > 0) {
                    context.pos--;
                }
            } else {
                if(*number == 0) context.pos--;
                bool neg = *number < 0;
                if(neg) *number = -*number;
                *number = (*number)/value*value*10 +
               (sym-'0')*value + (*number)%value;
                if(neg) *number = -*number;
                context.pos++;
            }
        } else if(sym == '-') {
            if(context.pos == 0 && *number > 0) {
                *number = -*number;
            }
        }
    }
    keyPressBreak:

    if(xEvent.type == ButtonPress) {
        int mx = xEvent.xbutton.x;
        int my = xEvent.xbutton.y;
        if(mx >= pos.x && mx <= pos.x + size.x &&
            my >= pos.y && my <= pos.y + size.y) {
            fprintf(stderr, "fwefwefw!\n");
            context.active = number;
            int newPos = (mx - pos.x - 5)/maxDigitWidth;
            context.pos = newPos > numberOfDigits? numberOfDigits:
                          newPos > 0 ? newPos :
                          0;
        } else if(context.active == number) {
            context.active = 0;
        }
    }
//    if(xEvent.type == Expose || ) {
        XSetForeground(xdisplay, p->gc, 0xff333333);
        XFillRectangle(xdisplay, p->window, p->gc, pos.x, pos.y,
                       size.x, size.y);
        if(context.active == number) {
            XSetForeground(xdisplay, p->gc, 0xff0000ff);
        } else {
            XSetForeground(xdisplay, p->gc, 0xffffffff);
        }
        XDrawRectangle(xdisplay, p->window, p->gc, pos.x, pos.y,
                       size.x, size.y);
        XSetForeground(xdisplay, p->gc, 0xffffffff);
        char string[20];
        int len = snprintf(string, 20, "%d", *number);
        Xutf8DrawString(xdisplay, p->window, xFontSet, p->gc,
                    pos.x+5 /*+ maxDigitWidth*digits*/,
                        pos.y+5 + maxDigitHeight, string, len);
        if(cursor && context.active == number) {
            XRectangle overallInk;
            XRectangle overallLog;

            Xutf8TextExtents(xFontSet, string, context.pos, &overallInk, &overallLog);
            XSetForeground(xdisplay, p->gc, 0xffffffff);
            XDrawLine(xdisplay, p->window, p->gc,
                      pos.x + 5 + overallInk.width, pos.y + 5,
                      pos.x + 5 + overallInk.width, pos.y + 5 + maxDigitHeight);
        }
        if(timeout) {
            cursor = !cursor;
        }
//    }
//    XPutImage(xdisplay, l->window, l->gc, l->x+5, l->y+5, width, height);
//    p->x += width+5;
//    p->maxHeight = MAX(p->maxHeight, height);
    feedbackSize(size);
}

void guiStartDrawing() {

    xdisplay = XOpenDisplay(NULL);
    assert(xdisplay);

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


    for(char d = '0'; d < '9'; d++) {
        XRectangle overallInk;
        XRectangle overallLog;
        Xutf8TextExtents(xFontSet, &d, 1, &overallInk, &overallLog);
        if(maxDigitWidth < overallLog.width) {
            maxDigitWidth = overallLog.width;
        }
        if(maxDigitHeight < overallLog.height) {
            maxDigitHeight = overallLog.height;
        }
    }



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
                 | ExposureMask | KeyPressMask);

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

    XFlush(xdisplay);

}

static int wait_fd(int fd, double seconds)
{
    struct timeval tv;
    fd_set in_fds;
    FD_ZERO(&in_fds);
    FD_SET(fd, &in_fds);
    tv.tv_sec = trunc(seconds);
    tv.tv_usec = (seconds - trunc(seconds))*1000000;
    return select(fd+1, &in_fds, 0, 0, &tv);
}
void XNextEventTimeout(Display *display, XEvent *event, double seconds)
{
    if (XPending(display) || wait_fd(ConnectionNumber(display),seconds)) {
       return XNextEvent(display, event);
    } else {
        timeout = true;
        xEvent.type = GenericEvent;
    }
}
