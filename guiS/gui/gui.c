#include "gui.h"
#include "guiglobals.h"
#include <X11/Xlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#define XK_LATIN1
#define XK_MISCELLANY
#include <X11/keysymdef.h>
#include <sys/select.h>
#include <stdlib.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

Point defaultGetPos() {
    fprintf(stderr, "getPos not set");
    abort();
}
void defaultFeedbackSize(Size s) {
    (void)s;
}

Point (*getPos)() = defaultGetPos;
void (*feedbackSize)(Size) = defaultFeedbackSize;

void guiDrawLine(Painter *a, int b, int c, int d, int e)
{
    XDrawLine(xdisplay, a->drawable, a->gc, b, c, d, e);
}

void guiDrawRectangle(Painter *a, int b, int c, int d, int e)
{
    XDrawRectangle(xdisplay, a->drawable, a->gc, b, c, d, e);
}
void guiFillRectangle(Painter *a, int b, int c, int d, int e)
{
//    fprintf(stderr, "filling rect (%d, %d) %dx%d\n", b, c, d,e);
    XFillRectangle(xdisplay, a->drawable, a->gc, b, c, d, e);
}

void guiSetForeground(Painter *a, unsigned long b)
{
    XSetForeground(xdisplay, a->gc, b);
}

void guiDrawTextWithLen(Painter *a, int b, int c, char *d, unsigned long e)
{
    Xutf8DrawString(xdisplay, a->drawable, xFontSet, a->gc, b, c, d, e);
}

void guiSetSize(Window win, uint w, uint h)
{
    XResizeWindow(xdisplay, win, w, h);
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
//                   size.width, size.height);
    if(xEvent.type != MotionNotify) {
        XSetForeground(xdisplay, p->gc, WhitePixel(xdisplay,
                                                   DefaultScreen(xdisplay)));
//        fprintf(stderr, "printing label %s %d %d\n",
//                text, pos.x+5 + overallLog.x, pos.y+5 - overallLog.y);
        Xutf8DrawString(xdisplay, p->drawable, xFontSet, p->gc,
                    pos.x+5 + overallLog.x, pos.y+5 - overallLog.y, text, len);
    }
    feedbackSize(size);
}
void guiLabelZT(Painter* p, char *text) {
    guiLabel(p, text, strlen(text));
}
bool guiButton(Painter *p, char* text, int len)
{
    Point pos = getPos();
    XRectangle overallInk;
    XRectangle overallLog;

    Xutf8TextExtents(xFontSet, text, len, &overallInk, &overallLog);
    Size size = {overallLog.width + 10,
                overallLog.height + 10};
    if(xEvent.type != MotionNotify) {
        XSetForeground(xdisplay, p->gc, 0xff555555);
//        XSetBackground(xdisplay, l->gc, 0xffff5555);
//        fprintf(stderr, "filling rect (%d, %d) %dx%d\n", pos.x, pos.y,
//                size.width, size.height);
        XFillRectangle(xdisplay, p->drawable, p->gc, pos.x, pos.y,
                       size.width, size.height);
        XSetForeground(xdisplay, p->gc, 0xffffffff);
        Xutf8DrawString(xdisplay, p->drawable, xFontSet, p->gc,
                    pos.x+5 + overallLog.x, pos.y+5 - overallLog.y, text, len);
    }
//    XPutImage(xdisplay, l->window, l->gc, l->x+5, l->y+5, width, height);
    bool res = false;
    if(xEvent.type == ButtonRelease) {
        int mx = xEvent.xbutton.x;
        int my = xEvent.xbutton.y;
        if(mx >= pos.x && mx <= pos.x + (int)size.width &&
            my >= pos.y && my <= pos.y + (int)size.height) {
            res = true;
        }
    }
//    p->x += width+5;
//    p->maxHeight = MAX(p->maxHeight, height);
    feedbackSize(size);
    return res;
}
bool guiButtonZT(Painter* p, char *text) {
    return guiButton(p, text, strlen(text));
}
bool guiToolButton(Painter *p, XImage *i, bool *consume) {
//    if(consume) *consume = false;
    Point pos = getPos();
    Size size = {i->width,
                 i->height};
    if(xEvent.type != MotionNotify) {
        XPutImage(xdisplay, p->drawable, p->gc, i,
                  0,0, pos.x, pos.y,
                  i->width, i->height);
    }
    bool res = false;
    if(xEvent.type == ButtonRelease
            || xEvent.type == ButtonPress) {
        int mx = xEvent.xbutton.x;
        int my = xEvent.xbutton.y;
        if(mx >= pos.x && mx <= pos.x + (int)size.width &&
            my >= pos.y && my <= pos.y + (int)size.height) {
            if(xEvent.type == ButtonRelease) {
                res = true;
            }
            if(consume) *consume = true;
        }
    }
    feedbackSize(size);
    return res;
}

bool guiNumberEdit(Painter *p, int digits, int *number, bool *consume) {
//    if(consume) *consume = false;
//    fprintf(stderr, "%d", *consume);
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
    bool res = false;

    if(xEvent.type == KeyPress && context.active == number) {
        if(consume) *consume = true;
        KeySym sym = XLookupKeysym(&xEvent.xkey, 0);
        fprintf(stderr, "%c %c! \n", (int)sym, XK_Right);
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
                    res = true;
                } else {
                    *number = (*number)/value/10*value + (*number)%value;
                    res = true;
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
                res = true;
            }
        } else if(sym == '-') {
            if(context.pos == 0 && *number > 0) {
                *number = -*number;
                res = true;
            }
        }
    }
    keyPressBreak:

    if(xEvent.type == ButtonPress || xEvent.type == ButtonRelease) {
        int mx = xEvent.xbutton.x;
        int my = xEvent.xbutton.y;
//        fprintf(stderr, "%d", *consume);
        if(mx >= pos.x && mx <= pos.x + (int)size.width &&
            my >= pos.y && my <= pos.y + (int)size.height) {
            if(consume) *consume = true;
            fprintf(stderr, "fwefwefw!\n");
            context.active = number;
//            fprintf(stderr, "%d", *consume);
            int newPos = (mx - pos.x - 5)/maxDigitWidth;
            context.pos = newPos > numberOfDigits? numberOfDigits:
                          newPos > 0 ? newPos :
                          0;
        } else if(context.active == number) {
            context.active = 0;
        }
    }
    if(xEvent.type != MotionNotify) {
        XSetForeground(xdisplay, p->gc, 0xff333333);
//        fprintf(stderr, "filling rect (%d, %d) %dx%d\n", pos.x, pos.y,
//                size.width, size.height);
        XFillRectangle(xdisplay, p->drawable, p->gc, pos.x, pos.y,
                       size.width, size.height);
//        fprintf(stderr, "%d", *consume);
        if(context.active == number) {
            XSetForeground(xdisplay, p->gc, 0xff0000ff);
        } else {
            XSetForeground(xdisplay, p->gc, 0xffffffff);
        }
        XDrawRectangle(xdisplay, p->drawable, p->gc, pos.x, pos.y,
                       size.width, size.height);
        XSetForeground(xdisplay, p->gc, 0xffffffff);
        char string[20];
        int len = snprintf(string, 20, "%d", *number);
        Xutf8DrawString(xdisplay, p->drawable, xFontSet, p->gc,
                    pos.x+5 /*+ maxDigitWidth*digits*/,
                        pos.y+5 + maxDigitHeight, string, len);
        if(cursor && context.active == number) {
            XRectangle overallInk;
            XRectangle overallLog;

            Xutf8TextExtents(xFontSet, string, context.pos, &overallInk, &overallLog);
            XSetForeground(xdisplay, p->gc, 0xffffffff);
            XDrawLine(xdisplay, p->drawable, p->gc,
                      pos.x + 5 + overallInk.width, pos.y + 5,
                      pos.x + 5 + overallInk.width, pos.y + 5 + maxDigitHeight);
//            fprintf(stderr, "%d", *consume);

        }
        if(xEvent.type == TimerEvent) {
            cursor = !cursor;
        }
  }
    feedbackSize(size);
//    fprintf(stderr, "%d", *consume);

    return res;
}

extern const char* appName;
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

    int numberOfDepths;
    int* depths = XListDepths(xdisplay, screen, &numberOfDepths);
    printf("Depth %d\n", xDepth);
    // Create the window
    rootWindow = XCreateSimpleWindow(xdisplay , DefaultRootWindow(xdisplay ), 0, 0,
                                        700, 700, 0, blackColor, blackColor);
    XSetWindowAttributes ats = {
     None, blackColor, CopyFromParent, whiteColor,
        ForgetGravity, NorthWestGravity, NotUseful,
        -1, 0, False, 0, 0, False, CopyFromParent, None
    };
//    rootWindow = XCreateWindow(xdisplay , DefaultRootWindow(xdisplay ), 0, 0,
//        700, 700, /*border*/ 0, /*depth*/4, InputOutput,
//            CopyFromParent, 0,0);

    // We want to get MapNotify events
    XSelectInput(xdisplay, rootWindow, StructureNotifyMask | ButtonPressMask
                 | ExposureMask | KeyPressMask
                 | ButtonMotionMask | ButtonReleaseMask);

    XStoreName(xdisplay, rootWindow, appName);
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

void guiNextEvent()
{
    if(redraw) {
        redraw = false;
        xEvent.type = Expose;
        return;
    }

    if (XPending(xdisplay) || wait_fd(ConnectionNumber(xdisplay),0.530)) {
       XNextEvent(xdisplay, &xEvent);
       if(xEvent.xany.window != rootWindow) {
            fprintf(stderr, "got wrong event %ud %ud\n", xEvent.type, xEvent.xany.window );
           abort();
       }

        if(xEvent.type == ConfigureNotify) {
            Size newSize = {
                xEvent.xconfigure.width,
                xEvent.xconfigure.height};
            rootWindowSize = newSize;
            fprintf(stderr, "reconfigure  %d x %d\n!!",
                    newSize.width,
                                          newSize.height);;
        }
       return;
    } else {
        xEvent.type = TimerEvent;
    }
}

Size guiGetSize()
{
    return rootWindowSize;
    Window w;
    int x, y;
    Size s;
    uint bw, d;
    XGetGeometry(xdisplay, rootWindow, &w,
                 &x, &y,
                 &s.width, &s.height,
                 &bw, &d);
    return s;
}


void guiRedraw()
{
    redraw = true;
}


void guiDoubleEdit(Painter *p, int digits, double *number)
{
    Point pos = getPos();
    static bool cursor = false;

    Size size = {maxDigitWidth*digits + 10,
                maxDigitHeight + 10};

    int numberOfDigits = 5;

    if(xEvent.type == ButtonPress) {
        int mx = xEvent.xbutton.x;
        int my = xEvent.xbutton.y;
        if(mx >= pos.x && mx <= pos.x + (int)size.width &&
            my >= pos.y && my <= pos.y + (int)size.height) {
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
//        fprintf(stderr, "filling rect (%d, %d) %dx%d\n", pos.x, pos.y,
//                size.width, size.height);
        XFillRectangle(xdisplay, p->drawable, p->gc, pos.x, pos.y,
                       size.width, size.height);
        if(context.active == number) {
            XSetForeground(xdisplay, p->gc, 0xff0000ff);
        } else {
            XSetForeground(xdisplay, p->gc, 0xffffffff);
        }
        XDrawRectangle(xdisplay, p->drawable, p->gc, pos.x, pos.y,
                       size.width, size.height);
        XSetForeground(xdisplay, p->gc, 0xffffffff);
        char string[20];
        char format[20];
        snprintf(format, 20, "%%%dd", numberOfDigits);

        int len = snprintf(string, 20, format, *number);
        Xutf8DrawString(xdisplay, p->drawable, xFontSet, p->gc,
                    pos.x+5 /*+ maxDigitWidth*digits*/,
                        pos.y+5 + maxDigitHeight, string, len);
        if(cursor && context.active == number) {
            XRectangle overallInk;
            XRectangle overallLog;

            Xutf8TextExtents(xFontSet, string, context.pos, &overallInk, &overallLog);
            XSetForeground(xdisplay, p->gc, 0xffffffff);
            XDrawLine(xdisplay, p->drawable, p->gc,
                      pos.x + 5 + overallInk.width, pos.y + 5,
                      pos.x + 5 + overallInk.width, pos.y + 5 + maxDigitHeight);
        }
        if(xEvent.type == TimerEvent) {
            cursor = !cursor;
        }
//  }
    feedbackSize(size);
}

#pragma GCC push_options
#pragma GCC optimize ("Ofast")
void guiFillRawRectangle(RawPicture *p, int x, int y, int w, int h, char r, char g, char b)
{
    assert(w >= 0);
    assert(h >= 0);
    assert(x >= 0);
    assert(y >= 0);
    if(x+w >= p->w) {
        w = p->w - x - 1;
    }
    if(y+h+1 >= p->h) {
        h = p->h - y - 2;
    }
    int color = rgb(r,g,b);
    for(int i = y; i <= y + h+1; i++) {
        for(int j = x; j <= x+w; j++) {
            *((int*)(p->data) + i*p->w + j) = color;
        }
    }
}
#pragma GCC pop_options

