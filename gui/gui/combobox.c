#include "gui.h"
#include "guiglobals.h"
#include "loadImage.h"
#include "combobox.h"
//#include <math.h>
//#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>



#include <SDL.h>
#include <Windows.h>
//#include <X11/Xlib.h>
//#define XK_LATIN1
//#define XK_MISCELLANY
//#include <X11/keysymdef.h>
//#include <sys/select.h>
//#include <stdlib.h>
Painter dropDownPainter = {/*0,0,*/0};
GuiWindow listWindow = 0;
bool guiComboBoxZT(Painter *p, const char * const *elements, int* current)
{
    STATIC(bool, init, (listWindow = guiMakeHiddenPopupWindow(),
                        dropDownPainter = guiMakePainter(listWindow),
                        true))


    Point pos = getPos();
    const char*const* c = elements;
    Rect overallLogMax = {0, 0, 0, 0};
    int numberOfElements = 0;
    while(*c != NULL) {
        numberOfElements++;
        Size overallLog = guiTextExtents(*c, (int)strlen(*c));
        overallLogMax.w = MAX(overallLog.w, overallLogMax.w);
        overallLogMax.h = MAX(overallLog.h, overallLogMax.h);
//        int newx = MIN(overallLogMax.x, overallLog.x);
//        int newy = MIN(overallLogMax.y, overallLog.y);
//        int newr = MAX(overallLogMax.x + overallLogMax.width,
//                       overallLog.x + overallLog.width);
//        int newb = MAX(overallLogMax.y + overallLogMax.height,
//                       overallLog.y + overallLog.height);
//        overallLogMax.x = newx;
//        overallLogMax.y = newy;
//        overallLogMax.width = newr - newx;
//        overallLogMax.height = newb - newy;
        c++;
    }

    overallLogMax.w += 10;//no fucking idea why
    STATIC(IMAGE*, triangle, loadResourceImagePngZT("triangle"));// = NULL;
//    if(triangle ==  NULL) {
//        triangle = ;
//    }
    Size iSize = imageSize(triangle);
    Size size = {overallLogMax.w + iSize.w + 10,
                MAX(overallLogMax.h, iSize.h) + 10};

    bool res = false;
//    if(event.xany.window == listWindow) {

//    }

    if(true || guiSameWindow(p, true)) {
        if(event.type ==ButtonRelease) {
            int mx = GET_X(event);
            int my = GET_Y(event);
            if(mx >= pos.x && mx <= pos.x + (int)size.w &&
                my >= pos.y && my <= pos.y + (int)size.h) {
    //            if(event.type == ButtonRelease) {
    //                res = true;
                    fprintf(stderr, "Mapping windown");
                    Point rect; SDL_GetWindowPosition(p->window, &rect.x, &rect.y);//guiGetRect();
                    guiMoveResizeWindow(listWindow, rect.x+pos.x, rect.y + pos.y + (int)size.h,
                                      size.w+1/*непонятно почему здесь окно получается на пиксель тоньше*/, (size.h)*numberOfElements);
                    guiShowWindow(listWindow);
                    guiRaiseWindow(listWindow);
                    context.numberOfDropDownElements = numberOfElements;
                    context.sizeOfDropDownElements = size;
                    context.active = current;
            }
            else {
                if(context.active == current) {
                    context.active = NULL;
                    fprintf(stderr, "Unmapping window %d %d %d %d %d %d\n",
                            mx, my, pos.x, pos.y
                            , pos.x+ (int)size.w, pos.y+ (int)size.h);
                    guiHideWindow(listWindow);
                }
            }
        }
        if(true/*event.type != MotionEvent*/) {
            guiSetForeground(p, 0xff555555);
            guiFillRectangle(p, (Rect){pos, size});
            guiSetForeground(p, 0xffffffff);
            guiDrawRectangle(p, (Rect){pos, size});
            guiSetForeground(p, 0xffffffff);
            int len = (int)strlen(elements[*current]);
            guiDrawText(p, elements[*current], len,
                (Point) {
                pos.x + 5 + overallLogMax.x,
                    pos.y + 5 - overallLogMax.y
            }, 0xffffffff);

            guiDrawImage(p, triangle,
                      pos.x+overallLogMax.x+overallLogMax.w,
                         pos.y+5);
            if(context.active == current) {
//                guiSetForeground(p, 0xffff5555);
//                guiClearWindow(&dropDownPainter);
                for(int i = 0; i <numberOfElements; i++) {
                    guiSetForeground(&dropDownPainter, 0xff333353);
                    guiFillRectangle(&dropDownPainter, (Rect){1, (i)*size.h+1,
                                   size.w-2, size.h-2});

                    int len = (int)strlen(elements[i]);
                    guiSetForeground(&dropDownPainter, 0xffffffff);
                    guiDrawText(&dropDownPainter,
                        elements[i], len,
                        (Point) {
                        5 + overallLogMax.x,
                            (i)*size.h + 5 - overallLogMax.y
                    },
                                0xffffffff);
                }
            }
        }
    }
    feedbackSize(size);
    return res;
}

void proccessComboBox() {
//    guiSetForeground(&dropDownPainter, 0xffff5555);
//    guiClearWindow(&dropDownPainter);

    if(event.type == ButtonRelease) {
        int mx = GET_X(event);
        int my = GET_Y(event);
//            , ,
        for(int i = 0; i < context.numberOfDropDownElements; i++) {
            Size size = context.sizeOfDropDownElements;
            if(mx >= 1 && mx <= (int)size.w-2  +  1 &&
                my >= (i)*(int)size.h+1 && my <= (int)size.h-2   +   (int)((i)*size.h+1)) {
//                res = true;
                *(int*)context.active = i;
            }
        }
        guiHideWindow(listWindow);
        guiRedraw();
//        SDL_UserEvent userevent = {TimerEvent, SDL_GetTicks(), 0, 0, 0, 0};
//        SDL_Event event; event.user = userevent;
//        SDL_PushEvent(&event);
    }
    if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        guiHideWindow(listWindow);
    }
}
