﻿#ifndef GUI_H
#define GUI_H
#include "backend.h"
#include "misc.h"
#include <stdbool.h>
#include <stddef.h>
#include "loadImage.h"
#ifdef _MSC_VER  
#define _Atomic
#endif
extern _Atomic bool cursor;
#ifdef __cplusplus
extern "C" {
#endif
extern Event event;
#define SCROLLBAR_THICKNESS 15

bool guiSameWindow(Painter* p);
Size guiTextExtents(/*Painter*p,*/const char *text, int len);
void guiDrawImageEx(Painter* p, IMAGE* i, int x, int y, Size *size);
void guiDrawImage(Painter* p, IMAGE* i, int x, int y);
//extern State state;
//extern Display * xdisplay;
//extern XFontStruct *xFontStruct;
//extern struct _XftFont *xFont;
extern GuiWindow rootWindow;
extern Painter rootWindowPainter;
//extern int xDepth;

typedef struct Point {
    i32 x;
    i32 y;
} Point;
typedef struct Size {
    u32 w;
    u32 h;
} Size;
typedef struct Rect {
    i32 x;
    i32 y;
    u32 width;
    u32 height;
} Rect;
bool pointInRect(Point p, Rect r);
Point getPos();
void feedbackSize(Size);
Size availableSize();

//Point getSavedPos();
//void savePos(int x, int y);

typedef struct RawPicture {
    char* data;
    size_t w;
    size_t h;
} RawPicture ;
// in interactive widgets p->drawable should be a Window
bool guiComboBoxZT(Painter *p, char const*const*elements, int* current);
bool guiIntField(Painter*p, int digits, int* number);
bool guiDoubleField(Painter*p, int digits, double* number);
bool guiButton(Painter *p, char* text, int len);
//bool guiButtonWindow(Painter *p, char* text, int len);
bool guiCheckBox(Painter* p, bool* v);
bool guiToolButton(Painter* p, IMAGE* i);
bool guiToolButtonEx(Painter* p, IMAGE* i, bool active, Size* desirableSize);
Size guiDrawText(Painter* p, const char *text, int len,
                 Point pos, i32 color);
Size guiDrawTextZT(Painter* p, char *text, Point pos, i32 color);
void guiLabel(Painter* p, char *text, int len);
void guiLabelZT(Painter* p, char *text);
void guiLabelZTWithBackground(Painter* p, char *text, bool back);
void guiLabelWithBackground(Painter* p, char *text, int len, bool back);
bool guiButtonZT(Painter* p, char *text);
bool guiScrollBar(Painter *p/*, Point pos*/, int length, double* value, double sliderFraction);
//bool guiSlider(Painter*, double* v, double start, double end);
bool resourseToolButtonEx(Painter*p, const char* name, bool active, Size *desirableSize);
bool resourseToolButton(Painter*p, const char* name);
bool standardResourseToolButton(Painter*p, char* name);

void guiDrawLine(Painter*, int, int, int, int);
void guiDrawRectangle(Painter*, int, int, int, int);
void guiFillRectangle(Painter *a, int b, int c, int d, int e);
void guiFillRawRectangle(RawPicture *p, int x, int y, int w, int h, char r, char g, char b);
void guiSetForeground(Painter*, unsigned long);
void guiDrawTextWithLen(Painter*, int, int, char*, unsigned long);
void guiSetSize(u32, u32);
Size guiGetSize();

static const unsigned int GuiDarkMagenta = 0x880088;
static inline unsigned int rgb(int r, int g, int b) {
    return r << 16 | g << 8 | b;
}
static inline unsigned int rgbf(double r, double g, double b) {
    return rgb((int)r*255, (int)g*255, (int)b*255);
}
static inline u32 gray(int gv) {
    return rgb(gv,gv,gv);
}

#ifdef __cplusplus
//extern "C"
#endif
void guiStartDrawing();
void guiNextEvent();
void guiRedraw();

#ifdef __cplusplus
} // extern "C"
#endif
#endif // GUI_H
