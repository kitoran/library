#include "gui.h"
#include "guiglobals.h"
#include "loadImage.h"
//#include <X11/Xlib.h>
//#include <math.h>
//#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
//#define XK_LATIN1
//#define XK_MISCELLANY
//#include <X11/keysymdef.h>
//#include <sys/select.h>
//#include <stdlib.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
bool guiComboBoxZT(Painter *p, const char * const *elements, int* current)
{
    static Window listWindow = None;
    static GC gc = None;
    if(listWindow == None) {
        listWindow = XCreateSimpleWindow(xdisplay, rootWindow, 0, 0,
                                             700, 700, 0,
                                                   WhitePixel(xdisplay,
                                                              DefaultScreen(xdisplay)),
                                                   WhitePixel(xdisplay,
                                                           DefaultScreen(xdisplay)));
        XSelectInput(xdisplay, listWindow, ButtonReleaseMask);
        gc = XCreateGC(xdisplay, listWindow, 0,0);

    }
//    if(xEvent.xany.window != p->drawable) {
//        if(xEvent.xany.window != listWindow) {
//            return false;
//        }
//        assert(xEvent.type == ButtonRelease);

//    }
    Point pos = getPos();
    char** c = elements;
    XRectangle overallLogMax = {0, 0, 0, 0};
    int numberOfElements = 0;
    while(*c != NULL) {
        numberOfElements++;
        XRectangle overallInk;
        XRectangle overallLog;
        Xutf8TextExtents(xFontSet, *c, strlen(*c), &overallInk, &overallLog);
        int newx = MIN(overallLogMax.x, overallLog.x);
        int newy = MIN(overallLogMax.y, overallLog.y);
        int newr = MAX(overallLogMax.x + overallLogMax.width,
                       overallLog.x + overallLog.width);
        int newb = MAX(overallLogMax.y + overallLogMax.height,
                       overallLog.y + overallLog.height);
        overallLogMax.x = newx;
        overallLogMax.y = newy;
        overallLogMax.width = newr - newx;
        overallLogMax.height = newb - newy;
        c++;
    }

    overallLogMax.width += 10;//no fucking idea why
    static XImage* triangle = NULL;
    if(triangle ==  NULL) {
        triangle = loadImageZT(GUI_RESOURCE_PATH, "triangle.png");
    }

    Size size = {overallLogMax.width + triangle->width + 10,
                MAX(overallLogMax.height, triangle->height) + 10};

    bool res = false;
//    if(xEvent.xany.window == listWindow) {
        if(xEvent.type == ButtonRelease && context.active == current) {
            int mx = xEvent.xbutton.x-pos.x;
            int my = xEvent.xbutton.y-pos.y - (int)size.height;
//            , ,
            for(int i = 0; i < numberOfElements; i++) {
                if(mx >= 1 && mx <= size.width-2  +  1 &&
                    my >= (i)*size.height+1 && my <= size.height-2   +   (i)*size.height+1) {
                    res = true;
                    *current = i;
                }
            }
        }
//    }

    if(xEvent.xany.window == p->drawable) {
        if(xEvent.type == ButtonRelease) {
            int mx = xEvent.xbutton.x;
            int my = xEvent.xbutton.y;
            if(mx >= pos.x && mx <= pos.x + (int)size.width &&
                my >= pos.y && my <= pos.y + (int)size.height) {
    //            if(xEvent.type == ButtonRelease) {
    //                res = true;
                    fprintf(stderr, "Mapping window\n");
                    XMoveResizeWindow(xdisplay, listWindow, pos.x, pos.y + (int)size.height,
                                      size.width+1/*непонятно почему здесь окно получается на пиксель тоньше*/, (size.height)*numberOfElements);
                    XSetBackground(xdisplay, gc, 0xffffffff);
                    XSetForeground(xdisplay, gc, 0xffffffff);
                    XDrawRectangle(xdisplay, listWindow, gc, 5,5,30,30);
                    XMapWindow(xdisplay, listWindow);
    //            }
                context.active = current;
    //            if(consume) *consume = true;
            }
            else {
                if(context.active == current) {
                    context.active = NULL;
                    fprintf(stderr, "Unmapping window %d %d %d %d %d %d\n",
                            mx, my, pos.x, pos.y
                            , pos.x+ (int)size.width, pos.y+ (int)size.height);
                    XUnmapWindow(xdisplay, listWindow);
                }
            }
        }
        if(xEvent.type != MotionNotify) {
            XSetForeground(xdisplay, p->gc, 0xff555555);
            XFillRectangle(xdisplay, p->drawable, p->gc, pos.x, pos.y,
                           size.width, size.height);
            XSetForeground(xdisplay, p->gc, 0xffffffff);
            XDrawRectangle(xdisplay, p->drawable, p->gc, pos.x, pos.y,
                           size.width, size.height);
            XSetForeground(xdisplay, p->gc, WhitePixel(xdisplay,
                                                       DefaultScreen(xdisplay)));
            int len = strlen(elements[*current]);
            Xutf8DrawString(xdisplay, p->drawable, xFontSet, p->gc,
                        pos.x+5 + overallLogMax.x, pos.y+5 - overallLogMax.y,
                            elements[*current], len);

            XPutImage(xdisplay, p->drawable, p->gc, triangle,
                      0,0, pos.x+overallLogMax.x+overallLogMax.width,
                      pos.y+5,
                      triangle->width, triangle->height);
            if(context.active == current) {
                XClearWindow(xdisplay, listWindow);
                for(int i = 0; i <numberOfElements; i++) {
                    XSetForeground(xdisplay, gc, 0xff333333);
                    XFillRectangle(xdisplay, listWindow, gc, 1, (i)*size.height+1,
                                   size.width-2, size.height-2);

                    int len = strlen(elements[i]);
                    XSetForeground(xdisplay, gc, 0xffffffff);
                    Xutf8DrawString(xdisplay, listWindow, xFontSet, gc,
                               5 + overallLogMax.x, (i)*size.height+5 - overallLogMax.y,
                                    elements[i], len);
                }
            }
        }
    }
    feedbackSize(size);
    return res;
}
