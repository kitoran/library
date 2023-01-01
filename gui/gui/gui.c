#include "gui.h"
#include "guiglobals.h"
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>

#include <layoutStack.h>

Point getPos() {
    LayoutVT** layout = (topLayout());
    return (*layout)->getPos(layout);
}
void feedbackSize(Size s) {
    LayoutVT** layout = topLayout();
    return (*layout)->feedbackSize(layout, s);
}
Size availableSize() {
    LayoutVT** layout = topLayout();
    return (*layout)->availableSize(layout);
}


#warning cache textures or surfaces
void guiLabelWithBackground(Painter* p, char *text, int len, bool back) {
    Point pos = getPos();
    Size extents = guiTextExtents(text, len);


    Size size = {extents.w + 10,
                extents.h + 10};

//    if(!(event.type == MotionEvent)) {
        if(back) {
            guiSetForeground(p,0);
            guiFillRectangle(p, pos.x, pos.y, size.w, size.h);
        }
        guiDrawText(p, text, len, STRU(Point, pos.x+5,pos.y+5/*+ extents.height*/),0xffffffff);
//    }
    feedbackSize(size);
}

void guiLabelZTWithBackground(Painter* p, char *text, bool back) {
    guiLabelWithBackground(p, text, strlen(text), back);
}
void guiLabel(Painter* p, char *text, int len)
{
    guiLabelWithBackground(p,text,len,false);
}
void guiLabelZT(Painter* p, char *text) {
    guiLabelZTWithBackground(p, text, false);
}
bool guiButton(Painter *p, char* text, int len)
{
//    if(!guiSameWindow(p)) return false;
    Point pos = getPos();
    Size overallLog = guiTextExtents(text, len);
    Size size = {overallLog.w + 10,
                overallLog.h + 10};
//    if(!(event.type == MotionEvent)) {
        guiSetForeground(p, 0xff555555);
//        XSetBackground(xdisplay, l->gc, 0xffff5555);
//        fprintf(stderr, "filling rect (%d, %d) %dx%d\n", pos.x, pos.y,
//                size.width, size.height);
        guiFillRectangle(p, pos.x, pos.y,
                       size.w, size.h);
        guiSetForeground(p, 0xffffffff);
        guiDrawText(p, text, len, STRU(Point,
                    pos.x+5 /*+ overallLog.x*/, pos.y+5 /*+ overallLog.height*/),
                    0xffffffff);
//    }
    bool res = false;
    if(event.type == ButtonRelease) {
        int mx = GET_X(event);
        int my = GET_Y(event);
        if(mx >= pos.x && mx <= pos.x + (int)size.w &&
            my >= pos.y && my <= pos.y + (int)size.h) {
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
bool guiToolButton(Painter *p, Image *i) {
    return guiToolButtonEx(p,i,false, NULL);
}
bool guiToolButtonEx(Painter *p, Image *i, bool active, Size* desirableSize) {
    if(!guiSameWindow(p)) return false;
    volatile Point pos = getPos();

    Size iSize = IMAGE_SIZE(i);
    Size size;
    if(desirableSize) {
        size = *desirableSize;
    } else {
        size = iSize;
    }
//    if(!(event.type == MotionEvent)) {
            guiSetForeground(p, active?0xffff9999:0xff000000);
            guiFillRectangle(p, pos.x, pos.y, size.w, size.h);
        guiDrawImageEx(p, i, pos.x, pos.y, &size);
//    }
    bool res = false;
    if(event.type == ButtonRelease
            || event.type == ButtonPress) {
        int mx = GET_X(event);
        int my = GET_Y(event);
        if(mx >= pos.x && mx <= pos.x + (int)size.w &&
            my >= pos.y && my <= pos.y + (int)size.h) {
            if(event.type == ButtonRelease) {
                res = true;
            }
        }
    }
    feedbackSize(size);
    return res;
}
typedef enum PermittedSymbols {intSymbols, doubleSymbols, allSymbols} PermittedSymbols;
bool guiAbstractField(Painter *p, int textWidth, void* id, PermittedSymbols symbols) {
    Point pos = getPos();
    Size size = {textWidth + 10,
                maxDigitHeight + 10};
    Rect rect = {pos.x, pos.y, size.w, size.h};
    guiSetClipRect(p, rect);
    bool res = false;
    void commit()
    {
        context.active = 0;
        context.editedString[context.editedStringLen] = 0;
//        sscanf(context.editedString, "%d", number);
        res = true;
    }
    int chosesnprintf(char* buffer) {
        switch(symbols) {
        case intSymbols:
            return snprintf(buffer,
                            MAX_INPUT_LEN,
                            "%d",
                            *(int*)id);
        case doubleSymbols:
            return snprintf(buffer,
                            MAX_INPUT_LEN,
                            "%lf",
                            *(double*)id);
        default:
            return snprintf(buffer,
                         MAX_INPUT_LEN,
                         "%s",
                         (char*)id);
        }
    }

    if(event.type == KeyPress && context.active == id) {
        GuiKeySym sym = GET_KEYSYM(event);
        DEBUG_PRINT(sym, "%d");
        DEBUG_PRINT(backspace, "%d");
//        DEBUG_PRINT(context.editedStringLen, "%d");
//        DEBUG_PRINT(context.editedString, "%s");
//        fprintf(stderr, "%c %c! \n", (int)sym, right);
        if(sym == rightKey) {
            if(context.pos < context.editedStringLen) {
                context.pos++;
            }
            cursor = true;
        } else if(sym == enter) {
            commit();
        } else if(sym == leftKey) {
            if(context.pos > 0) {
                context.pos--;
            }
            cursor = true;
        } else if(sym == backspace) {
            if(context.pos == 0) {
                goto keyPressBreak;
            }
            for(int i = context.pos-1; i < context.editedStringLen-1; i++) {
                context.editedString[i] = context.editedString[i+1];
            }
            context.editedStringLen--;
            context.pos--;
        } if(sym == delete) {
            if(context.pos == context.editedStringLen) {
                goto keyPressBreak;
            }
            for(int i = context.pos; i < context.editedStringLen-1; i++) {
                context.editedString[i] = context.editedString[i+1];
            }
            context.editedStringLen--;
        } else if(symbols == intSymbols? ((sym >= '0' && sym <= '9') || sym == '-') :
                  symbols == doubleSymbols? ((sym >= '0' && sym <= '9') || sym == '-'||sym=='.'):
                  true) {
            if(context.editedStringLen == MAX_INPUT_LEN) {
                goto keyPressBreak;
            }
            for(int i = context.editedStringLen; i > context.pos; i--) {
                context.editedString[i] = context.editedString[i-1];
            }
            context.editedStringLen++;
            context.editedString[context.pos] = sym;
            context.pos++;
        }
    }
    keyPressBreak:

    {
        guiSetForeground(p, 0xff333333);
        guiFillRectangle(p, pos.x, pos.y,
                       size.w, size.h);
        if(context.active == id) {
            guiSetForeground(p, 0xff0000ff);
        } else {
            guiSetForeground(p, 0xffffffff);
        }
        guiDrawRectangle(p, pos.x, pos.y,
                       size.w, size.h);
        guiSetForeground(p, 0xffffffff);
        if(context.active == id) {
            guiDrawText(p, context.editedString, context.editedStringLen,
                     STRU(Point, pos.x+5, pos.y+5), 0xffffffff);
        } else {
            char stringN[MAX_INPUT_LEN+1];
            int len = chosesnprintf(stringN);
            guiDrawText(p, stringN, len,
                     STRU(Point, pos.x+5, pos.y+5), 0xffffffff);
        }
        if(cursor && context.active == id) {
            Size overallInk = guiTextExtents(context.editedString, context.pos);//, &overallInk, &overallLog);
            guiSetForeground(p, 0xffffffff);
            guiDrawLine(p,
                      pos.x + 5 + overallInk.w, pos.y + 5,
                      pos.x + 5 + overallInk.w, pos.y + 5 + maxDigitHeight);

        }
    }
    if(event.type == ButtonPress || event.type == ButtonRelease) {
        int mx = GET_X(event);
        int my = GET_Y(event);
        if(mx >= pos.x && mx <= pos.x + (int)size.w &&
            my >= pos.y && my <= pos.y + (int)size.h) {
            if(context.active != id) {
                context.editedStringLen = chosesnprintf(context.editedString);
            }
            context.active = id;

            int newPosPx, newPos;
            int partWidth = mx-(pos.x+5);
            guiGetTextPart(context.editedString, partWidth, &newPosPx, &newPos);
            int biggerPosPx = guiTextExtents(context.editedString, newPos+1).w;
            if(biggerPosPx - partWidth < partWidth - newPosPx) newPos++;

//            int newPos = (mx - pos.x - 5)/maxDigitWidth;
            DEBUG_PRINT(partWidth, "%d");
            DEBUG_PRINT(newPosPx, "%d");
            DEBUG_PRINT(biggerPosPx, "%d");
            DEBUG_PRINT(newPos, "%d");
            context.pos = MIN(MAX(newPos, 0), context.editedStringLen);
        } else if(context.active == id) {
            commit();
        }
    }
    if(event.type == TimerEvent) {
        cursor = !cursor;
    }

    guiUnsetClipRect(p);
    feedbackSize(size);
    return res;
}

bool guiIntField(Painter *p, int digits, int *number) {
    bool res = guiAbstractField(p, maxDigitWidth*digits, number, intSymbols);
    if(res) {
        sscanf(context.editedString, "%d", number);
    }
    return res;
}
bool guiDoubleField(Painter *p, int digits, double *number) {
    bool res = guiAbstractField(p, maxDigitWidth*(digits+1), number, doubleSymbols);
    if(res) {
        sscanf(context.editedString, "%lf", number);
    }
    return res;
}
extern const char* appName;
const char* __attribute__((weak))  appName = "gui application";



Size guiGetSize()
{
    return rootWindowSize;
}

void guiRedraw()
{
    redraw = true;
}

#pragma GCC push_options
#pragma GCC optimize ("Ofast")
void guiFillRawRectangle(RawPicture *p, int x, int y, int w, int h, char r, char g, char b)
{
    assert(w >= 0);
    assert(h >= 0);
    assert(x >= 0);
    assert(y >= 0);
    if(x+w >= (int)(p->w)) {
        w = p->w - x - 1;
    }
    if(y+h+1 >= (int)(p->h)) {
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


Size guiDrawTextZT(Painter *p, char *text, Point pos, i32 color)
{
    return guiDrawText(p, text, strlen(text), pos, color);
}

bool guiCheckBox(Painter *p, bool *v)
{
    Point pos = getPos();
    Size size = {10,
                 10};
    bool res = false;
    if(event.type == ButtonPress) {
        int mx = GET_X(event);
        int my = GET_Y(event);
        if(mx >= pos.x && mx <= pos.x + (int)size.w &&
            my >= pos.y && my <= pos.y + (int)size.h) {
            *v = !*v;
            res = true;
        }
    }

    if(*v) {
        guiDrawLine(p, pos.x, pos.y+size.h,
                  pos.x+size.w, pos.y);
        guiDrawLine(p, pos.x, pos.y,
                  pos.x+size.w, pos.y+size.h);
    } else {
        guiSetForeground(p, 0xff000000);
        guiFillRectangle(p, pos.x, pos.y,
                       size.w, size.h);
    }
    guiSetForeground(p, 0xffffffff);
    guiDrawRectangle(p, pos.x, pos.y,
                   size.w, size.h);
    feedbackSize(size);
    return res;
}
#define SCROLLBAR_SPEED 0.1
// very nonfinal api
// maybe ints instead of doubles
_Bool guiScrollBar(Painter *p/*, Point pos*/, int length, double* value, double sliderFraction) {
    Point pos = getPos();
    Size size = {length, SCROLLBAR_THICKNESS};
    _Bool res = false;
    STATIC(IMAGE*, left, loadImageZT(GUI_RESOURCE_PATH, "leftButton.png"));
    STATIC(IMAGE*, right, loadImageZT(GUI_RESOURCE_PATH, "rightButton.png"));
    double maxValue = 1-sliderFraction;
    Size buttonSize = {SCROLLBAR_THICKNESS, SCROLLBAR_THICKNESS};
    {
        __attribute__ ((cleanup(popLayout))) ExactLayout l = makeExactLayout(pos);
        pushLayout(&l);
        if(guiToolButtonEx(p, left, false, &buttonSize)) {
            res = true;
            *value = MAX(*value - SCROLLBAR_SPEED, 0);
        }
        l.exactPos = STRU(Point, pos.x+length-SCROLLBAR_THICKNESS, pos.y);
        if(guiToolButtonEx(p, right, false, &buttonSize)) {
            res = true;
            *value = MIN(*value + SCROLLBAR_SPEED, maxValue);
        }
    }
    int internalStart = pos.x+SCROLLBAR_THICKNESS;
    int internalLength = length - 2*SCROLLBAR_THICKNESS;

    Rect slider = {pos.x + SCROLLBAR_THICKNESS + *value*internalLength,
                    pos.y,
                   sliderFraction*internalLength, SCROLLBAR_THICKNESS};
//    if(event.type != MotionEvent || context.active == value) {
        guiSetForeground(p, 0xff111111);
        guiFillRectangle(p, internalStart, pos.y, internalLength, SCROLLBAR_THICKNESS);
        guiSetForeground(p, 0xff555555);
        guiFillRectangle(p, slider.x, slider.y, slider.width, slider.height);
//    }



    static Point dragStart = {-1, -1};
    static int dragStartPos = -1;
    if(event.type == ButtonPress) {
        Point mpos = STRU(Point, GET_X(event), GET_Y(event));
        Rect leftPart = {internalStart, pos.y, slider.x-internalStart, SCROLLBAR_THICKNESS};
        if(pointInRect(mpos, leftPart)) {
            res = true;
            *value = MAX(*value - sliderFraction, 0);
        }
        Rect rightPart = {slider.x+slider.width,
                           pos.y,
                          pos.x+length-SCROLLBAR_THICKNESS, SCROLLBAR_THICKNESS};
        if(pointInRect(mpos, rightPart)) {
            res = true;
            *value = MIN(*value + sliderFraction, maxValue);
        }
        if(pointInRect(mpos, slider)) {
            context.active = value;
            dragStart = mpos;
            dragStartPos = slider.x;
        }
//        if(context.active == value && !pointInRect(mpos, (Rect){pos, size})) {
//            context.active = NULL;
//        }
    }
    if(context.active == value &&
            event.type == MotionEvent) {
        Point mpos = STRU(Point, GET_X(event), GET_Y(event));
        *value = (dragStartPos + mpos.x-dragStart.x)*1.0/internalLength;
        *value = CLAMP(*value, 0, maxValue);
        res = true;
    }
    if(context.active == value &&
            event.type == ButtonRelease) {
        context.active = NULL;
        dragStart = (Point){-1,-1};
        dragStartPos = -1;
    }

    feedbackSize(size);
    return res;
}

_Bool pointInRect(Point p, Rect r)
{
    return p.x >= r.x && p.x <= r.x+(i32)r.width &&
            p.y >= r.y && p.y <= r.y +(i32)r.height;
}
