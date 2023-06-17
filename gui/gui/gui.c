#include "gui.h"
#include "guiglobals.h"
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stb_ds.h>
//#include <sys/select.h>
#include <stdlib.h>

#include <layoutStack.h>
//enum {
//    topRight,
//    center
//} alignment;
Point getPos() {
    LayoutVT** layout = (topLayout());
    Point corner = (*layout)->getPos(layout);
/*    if(alignment == topRight)*/ return corner;
//    if(alignment == center) {
//        Size allotted = (*layout)->minimumAllottedSize(layout);
//        return (Point){corner.x + (allotted.w+s.w + 1)/2,
//                    corner.y + (allotted.h+s.h + 1)/2};
//    }
//    ASSERT(false, "alignment not handled");
}
void feedbackSize(Size s) {
    LayoutVT** layout = topLayout();
    (*layout)->feedbackSize(layout, s);
}
Size availableSize() {
    LayoutVT** layout = topLayout();
    return (*layout)->availableSize(layout);
}

// TODO cache textures or surfaces
void guiLabelWithBackground(Painter* p, char *text, int len, bool back) {
    Point pos = getPos();

Size extents;
Size size ;
//    if(!(event.type == MotionEvent)) {
    if(back) {
        extents = guiTextExtents(text, len);
        size = (Size){extents.w + 10,
                    extents.h + 10};
        guiSetForeground(p,0);
        guiFillRectangle(p, (Rect){pos, size});
    }
    extents = guiDrawText(p, text, len, (Point){ pos.x+5,pos.y+5/*+ extents.height*/},0xffffffff);
    size = (Size){extents.w + 10,
                extents.h + 10};
//    }
    feedbackSize(size);
}

void guiLabelZTWithBackground(Painter* p, char *text, bool back) {
    guiLabelWithBackground(p, text, (int)strlen(text), back);
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
        guiFillRectangle(p, (Rect){pos, size});
        guiSetForeground(p, 0xffffffff);
        guiDrawText(p, text, len, (Point) {
            pos.x + 5 /*+ overallLog.x*/, pos.y + 5 /*+ overallLog.height*/
        },
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
    return guiButton(p, text, (int)strlen(text));
}
bool guiToolButton(Painter *p, char* imagePath) {
    return guiToolButtonEx(p,imagePath,false, false, NULL, 0);
}
bool guiToolButtonEx(Painter *p, char* imagePath, bool activatable, bool active, const Size* desirableSizeFull, int margins) {
    if(!guiSameWindow(p, true)) return false;

    static struct {
        char* key;
        IMAGE* value;
    } *map = NULL;

    IMAGE* image;
    int index = (int)shgeti(map, imagePath);
    if(index == -1) {
        image = loadImageZT(imagePath);
        shput(map, imagePath, image);
    } else {
        image = map[index].value;
    }

    volatile Point pos = getPos();

    Size iSize = imageSize(image);
    Size size;
    if(desirableSizeFull) {
        size = *desirableSizeFull;
    } else {
        size = (Size){iSize.h+margins*2, iSize.w+margins*2};
    }
    Size imageSizeResult = (Size){size.h-margins*2, size.w-margins*2};
//    if(!(event.type == MotionEvent)) {
            guiSetForeground(p, 0xffffffff);
         if(active)   guiFillRectangle(p, (Rect){pos, size});
        guiDrawImageEx(p, image, pos.x+margins, pos.y+margins, &imageSizeResult);
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
static void commit(bool* res)
{
    context.active = 0;
    context.editedString[context.editedStringLen] = 0;
    //        sscanf(context.editedString, "%d", number);
    *res = true;
}
int chosesnprintf(PermittedSymbols symbols, void* id, char* buffer) {
    switch(symbols) {
    case intSymbols:
        return snprintf(buffer,
                        MAX_STRING_LEN,
                        "%d",
                        *(int*)id);
    case doubleSymbols:
        return snprintf(buffer,
                        MAX_STRING_LEN,
                        "%lf",
                        *(double*)id);
    default:
        return snprintf(buffer,
                     MAX_STRING_LEN,
                     "%s",
                     (char*)id);
    }
}
bool guiAbstractField(Painter *p, int textWidth, void* id, PermittedSymbols symbols) {
    Point pos = getPos();
    Size size = {textWidth + 10,
                maxDigitHeight + 10};
    Rect rect = {pos.x, pos.y, size.w, size.h};
//    Rect innerRect = {pos.x+1, pos.y+1, size.w-2, size.h-2};
    guiSetClipRect(p, rect);
    bool res = false;
    


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
            commit(&res);
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
        } if(sym == deleteKey) {
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
            if(context.editedStringLen == MAX_STRING_LEN) {
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
    context.editedString[context.editedStringLen] = 0; // TODO: this is a hack
    if(event.type == ButtonPress || event.type == ButtonRelease) {
        int mx = GET_X(event);
        int my = GET_Y(event);
        if(mx >= pos.x && mx <= pos.x + (int)size.w &&
            my >= pos.y && my <= pos.y + (int)size.h) {
            if(context.active != id) {
                context.editedStringLen = chosesnprintf(symbols, id, context.editedString);
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
            commit(&res);
        }
    }

    {
        guiSetForeground(p, 0xff333363);
        guiFillRectangle(p, (Rect){pos, size});
        guiSetForeground(p, 0xffffffff);
        if(context.active == id) {
            guiDrawText(p, context.editedString, context.editedStringLen,
                     (Point){ pos.x+5, pos.y+5}, 0xffffffff);
        } else {
            char stringN[MAX_STRING_LEN+1];
            int len = chosesnprintf(symbols, id, stringN);
            guiDrawText(p, stringN, len,
                     (Point){ pos.x+5, pos.y+5}, 0xffffffff);
        }
        if(cursor && context.active == id) {
            Size overallInk = guiTextExtents(context.editedString, context.pos);//, &overallInk, &overallLog);
            guiSetForeground(p, 0xffffffff);
            guiDrawLine(p,
                      pos.x + 5 + overallInk.w, pos.y + 5,
                      pos.x + 5 + overallInk.w, pos.y + 5 + maxDigitHeight);

        }
        if(context.active == id) {
            guiSetForeground(p, 0xff0000ff);
        } else {
            guiSetForeground(p, 0xffffffff);
        }
        guiDrawRectangle(p, (Rect){pos, size});
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
#ifndef _MSC_VER
const char* __attribute__((weak))  appName = "gui application";
#endif

void guiRedraw()
{
    redraw = true;
}
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("Ofast")
#endif
void guiFillRawRectangle(RawPicture *p, int x, int y, int w, int h, char r, char g, char b)
{
    assert(w >= 0);
    assert(h >= 0);
    assert(x >= 0);
    assert(y >= 0);
    if(x+w >= (int)(p->w)) {
        w = (int)(p->w) - x - 1;
    }
    if(y+h+1 >= (int)(p->h)) {
        h = (int)(p->h) - y - 2;
    }
    int color = rgb(r,g,b);
    for(int i = y; i <= y + h+1; i++) {
        for(int j = x; j <= x+w; j++) {
            *((int*)(p->data) + i*p->w + j) = color;
        }
    }
}
#ifdef __GNUC__
#pragma GCC pop_options
#endif

Size guiDrawTextZT(Painter *p, const char *text, Point pos, i32 color)
{
    return guiDrawText(p, text, (int)strlen(text), pos, color);
}

bool guiCheckBox(Painter *p, bool *v)
{
    Point pos = getPos();
    Size size = {10,
                 guiFontHeight() + 10};
    Size boxSize = {10,
                    10};
    Rect boxRect = {
        .x = pos.x,
        .y = pos.y + (size.h-boxSize.h+1)/2,
        .size = boxSize
    };
    bool res = false;
    if(event.type == ButtonPress) {
        int mx = GET_X(event);
        int my = GET_Y(event);
        if(pointInRect((Point){mx, my}, boxRect)) {
            *v = !*v;
            res = true;
        }
    }

    if(*v) {
        STATIC(Image*, mark, loadImageZT(GUI_RESOURCE_PATH "/check.png"));
        guiDrawImage(p, mark, boxRect.x, boxRect.y);
    } else {
        guiSetForeground(p, 0xff000000);
        guiFillRectangle(p, boxRect);
        guiSetForeground(p, 0xffffffff);
        guiDrawRectangle(p, boxRect);
    }
    feedbackSize(size);
    return res;
}
#define SCROLLBAR_SPEED 0.1
// very nonfinal api
// maybe ints instead of doubles
/*
_Bool guiHorizontalScrollBar(Painter *p, int length, double* value, double sliderFraction) {
    Point pos = getPos();
    Size size = {length, SCROLLBAR_THICKNESS};
    _Bool res = false;
    STATIC(IMAGE*, left, loadImageZT(GUI_RESOURCE_PATH, "leftButton.png"));
    STATIC(IMAGE*, right, loadImageZT(GUI_RESOURCE_PATH, "rightButton.png"));
    double maxValue = 1-sliderFraction;
    Size buttonSize = {SCROLLBAR_THICKNESS, SCROLLBAR_THICKNESS};
    {
        ExactLayout l = makeExactLayout(pos);
        pushLayout(&l);
        if(guiToolButtonEx(p, left, false, &buttonSize)) {
            res = true;
            *value = MAX(*value - SCROLLBAR_SPEED, 0);
        }
        l.exactPos = (Point){ pos.x+length-SCROLLBAR_THICKNESS, pos.y};
        if(guiToolButtonEx(p, right, false, &buttonSize)) {
            res = true;
            *value = MIN(*value + SCROLLBAR_SPEED, maxValue);
        }
        popLayout();
    }
    int internalStart = pos.x+SCROLLBAR_THICKNESS;
    int internalLength = length - 2*SCROLLBAR_THICKNESS;

    Rect slider = {pos.x + SCROLLBAR_THICKNESS + *value*internalLength,
                    pos.y,
                   sliderFraction*internalLength, SCROLLBAR_THICKNESS};
    guiSetForeground(p, 0xff111111);
    guiFillRectangle(p, internalStart, pos.y, internalLength, SCROLLBAR_THICKNESS);
    guiSetForeground(p, 0xff555555);
    guiFillRectangle(p, slider.x, slider.y, slider.w, slider.h);
    static Point dragStart = {-1, -1};
    static int dragStartPos = -1;
    if(event.type == ButtonPress) {
        Point mpos = (Point){ GET_X(event), GET_Y(event)};
        Rect leftPart = {internalStart, pos.y, slider.x-internalStart, SCROLLBAR_THICKNESS};
        if(pointInRect(mpos, leftPart)) {
            res = true;
            *value = MAX(*value - sliderFraction, 0);
        }
        Rect rightPart = {slider.x+slider.w,
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
    }
    if(context.active == value &&
            event.type == MotionEvent) {
        Point mpos = (Point){ GET_X(event), GET_Y(event)};
        *value = (dragStartPos + mpos.x-dragStart.x-
                  internalStart)*1.0/internalLength;
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
_Bool guiVerticalScrollBar(Painter *p, int length, double* value, double sliderFraction) {
    Point pos = getPos();
    Size size = {SCROLLBAR_THICKNESS, length};
    _Bool res = false;
    STATIC(IMAGE*, up, loadImageZT(GUI_RESOURCE_PATH, "upButton.png"));
    STATIC(IMAGE*, down, loadImageZT(GUI_RESOURCE_PATH, "downButton.png"));
    double maxValue = 1-sliderFraction;
    Size buttonSize = {SCROLLBAR_THICKNESS, SCROLLBAR_THICKNESS};
    {
        ExactLayout l = makeExactLayout(pos);
        pushLayout(&l);
        if(guiToolButtonEx(p, up, false, &buttonSize)) {
            res = true;
            *value = MAX(*value - SCROLLBAR_SPEED, 0);
        }
        l.exactPos = (Point){ pos.x, pos.y+length-SCROLLBAR_THICKNESS };
        if(guiToolButtonEx(p, down, false, &buttonSize)) {
            res = true;
            *value = MIN(*value + SCROLLBAR_SPEED, maxValue);
        }
        popLayout();
    }
    int internalStart = pos.y+SCROLLBAR_THICKNESS;
    int internalLength = length - 2*SCROLLBAR_THICKNESS;

    Rect slider = {pos.x,
                    pos.y + SCROLLBAR_THICKNESS + *value*internalLength,
                   SCROLLBAR_THICKNESS, sliderFraction*internalLength};
    guiSetForeground(p, 0xff111111);
    guiFillRectangle(p, pos.x, internalStart, SCROLLBAR_THICKNESS, internalLength);
    guiSetForeground(p, 0xff555555);
    guiFillRectangle(p, slider.x, slider.y, slider.w, slider.h);
    static Point dragStart = {-1, -1};
    static int dragStartPos = -1;
    if(event.type == ButtonPress) {
        Point mpos = (Point){ GET_X(event), GET_Y(event)};
        Rect topPart = {pos.x, internalStart, SCROLLBAR_THICKNESS, slider.x-internalStart};
        if(pointInRect(mpos, topPart)) {
            res = true;
            *value = MAX(*value - sliderFraction, 0);
        }
        Rect bottomPart = {slider.x,
                           pos.y+slider.h,
                          SCROLLBAR_THICKNESS,
                          pos.y+length-SCROLLBAR_THICKNESS};
        if(pointInRect(mpos, bottomPart)) {
            res = true;
            *value = MIN(*value + sliderFraction, maxValue);
        }
        if(pointInRect(mpos, slider)) {
            context.active = value;
            dragStart = mpos;
            dragStartPos = slider.y;
        }
    }
    if(context.active == value &&
            event.type == MotionEvent) {
        Point mpos = (Point){ GET_X(event), GET_Y(event)};
        *value = (dragStartPos + mpos.y-dragStart.y-
                  internalStart)*1.0/internalLength;
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
}*/
_Bool guiScrollBar(Painter *p, int length, double* value, double sliderFraction, bool ho) {
    typedef struct AbstractPoint {
        int across;
        int along;
    }AbstractPoint;
    typedef struct AbstractSize {
        int across;
        int along;
    }AbstractSize;
    typedef struct AbstractRect {
        AbstractPoint pos;
        AbstractSize size;
    }AbstractRect;
#define toConcreteRect(a) ho?(Rect){{a.pos.along, a.pos.across}, {a.size.along, a.size.across}}:(Rect){{a.pos.across, a.pos.along}, {a.size.across, a.size.along}}
#define toConcreteSize(a) ho?(Size){a.along, a.across}:(Size){a.across, a.along}
#define toAbstractPoint(a) ho?(AbstractPoint){a.y, a.x}:(AbstractPoint){a.x, a.y}
#define toConcretePoint(a) ho?(Point){a.along, a.across}:(Point){a.across, a.along}
    Point pos = getPos();
    AbstractPoint apos = toAbstractPoint(pos);
    AbstractSize size = {SCROLLBAR_THICKNESS, length};
    _Bool res = false;
//    STATIC(IMAGE*, up, loadImageZT(GUI_RESOURCE_PATH, "upButton.png"));
//    STATIC(IMAGE*, down, loadImageZT(GUI_RESOURCE_PATH, "downButton.png"));
//    STATIC(IMAGE*, left, loadImageZT(GUI_RESOURCE_PATH, "leftButton.png"));
//    STATIC(IMAGE*, right, loadImageZT(GUI_RESOURCE_PATH, "rightButton.png"));
    char* up = GUI_RESOURCE_PATH "/upButton.png";
    char* down = GUI_RESOURCE_PATH "/downButton.png";
    char* left = GUI_RESOURCE_PATH "/leftButton.png";
    char* right = GUI_RESOURCE_PATH "/rightButton.png";

    char* less = ho?left:up;
    char* more = ho?right:down;
    double maxValue = 1-sliderFraction;
    Size buttonSize = {SCROLLBAR_THICKNESS, SCROLLBAR_THICKNESS};
    {
        ExactLayout l = makeExactLayout(pos);
        pushLayout(&l);

        if(guiToolButtonEx(p, less, false, false, &buttonSize, 0)) {
            res = true;
            *value = MAX(*value - SCROLLBAR_SPEED, 0);
        }
        AbstractPoint moreButtonPos = { apos.across, apos.along+length-SCROLLBAR_THICKNESS };
        l.exactPos = toConcretePoint(moreButtonPos);
        if(guiToolButtonEx(p, more, false, false, &buttonSize, 0)) {
            res = true;
            *value = MIN(*value + SCROLLBAR_SPEED, maxValue);
        }
        popLayout();
    }
    int internalStart = apos.along+SCROLLBAR_THICKNESS;
    int internalLength = length - 2*SCROLLBAR_THICKNESS;
//    double maxValue = MAX(*value + sliderFraction, 1);
    double sliderEarly = MAX(*value,0);
    AbstractRect slider = {apos.across,
                    (int)(apos.along + SCROLLBAR_THICKNESS + sliderEarly*internalLength),
                   SCROLLBAR_THICKNESS, (int)(MIN(sliderFraction, 1-sliderEarly)*internalLength)};
    guiSetForeground(p, 0xff111111);
    AbstractRect internalRect = {apos.across, internalStart, SCROLLBAR_THICKNESS, internalLength};
    guiFillRectangle(p, toConcreteRect(internalRect));
    guiSetForeground(p, 0xff555555);
    guiFillRectangle(p, toConcreteRect(slider));
    static AbstractPoint dragStart = {-1, -1};
    static int dragStartPos = -1;
    if(event.type == ButtonPress) {
        Point mpos = (Point){ GET_X(event), GET_Y(event)};
        AbstractRect lessPart = {apos.across, internalStart, SCROLLBAR_THICKNESS, slider.pos.along-internalStart};
        if(pointInRect(mpos, toConcreteRect(lessPart))) {
            res = true;
            *value = MAX(*value - sliderFraction, 0);
        }
        AbstractRect morePart = {slider.pos.across,
                           slider.pos.along+slider.size.along,
                          SCROLLBAR_THICKNESS,
                          apos.along+length-SCROLLBAR_THICKNESS-(slider.pos.along+slider.size.along)};
        if(pointInRect(mpos, toConcreteRect(morePart))) {
            res = true;
            *value = MIN(*value + sliderFraction, maxValue);
        }
        if(pointInRect(mpos, toConcreteRect(slider))) {
            context.active = value;
            dragStart = toAbstractPoint(mpos);
            dragStartPos = slider.pos.along;
        }
    }
    if(context.active == value &&
            event.type == MotionEvent) {
        AbstractPoint ampos = toAbstractPoint(((Point){ GET_X(event), GET_Y(event)}));
        *value = (dragStartPos + ampos.along-dragStart.along-
                  internalStart)*1.0/internalLength;
        *value = CLAMP(*value, 0, maxValue);
        res = true;
    }
    if(context.active == value &&
            event.type == ButtonRelease) {
        context.active = NULL;
        dragStart = (AbstractPoint){-1,-1};
        dragStartPos = -1;
    }

    feedbackSize(toConcreteSize(size));
    return res;
}

//bool standardResourseToolButton(Painter*p, char* name, bool active, Size* desirableSize) {
//    static struct {
//        char* key;
//        IMAGE* value;
//    } *map = NULL;

//    IMAGE* image;
//    int index = (int)shgeti(map, name);
//    if(index == -1) {
//        image = loadImageZT(GUI_RESOURCE_PATH, name);
//        shput(map, name, image);
//    } else {
//        image = map[index].value;
//    }
//    return guiToolButtonEx(p, image, active, desirableSize);
//}
//bool resourseToolButton(Painter*p, const char* name) {
//   return resourseToolButtonEx(p, name, false, NULL);
//}
//bool resourseToolButtonEx(Painter*p, const char* name, bool active, Size* desirableSize) {
//    static struct {
//        char* key;
//        IMAGE* value;
//    } *map = NULL;

//    IMAGE* image;
//    int index = (int)shgeti(map, name);
////    fprintf(stderr, "%d, index", index);
//    if(index == -1) {
//        image = loadLocalImageZT(name);
//        shput(map, name, image);
//    } else {
//        image = map[index].value;
//    }
//    return guiToolButtonEx(p, image, active, desirableSize);
//}

_Bool pointInRect(Point p, Rect r)
{
    return p.x >= r.x && p.x <= r.x+(i32)r.w &&
            p.y >= r.y && p.y <= r.y +(i32)r.h;
}


void guiStartDrawing() {
    guiStartDrawingEx(true);
}
