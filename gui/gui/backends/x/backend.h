﻿#ifndef XBACKEND_H
#define XBACKEND_H

#ifdef SDL
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
//union SDL_Event;//struct SDL_Event;
typedef SDL_Event Event;
typedef SDL_Window* GuiWindow;
enum {
    MotionEvent = SDL_MOUSEMOTION,
    ButtonRelease = SDL_MOUSEBUTTONUP,
    ButtonPress = SDL_MOUSEBUTTONDOWN,
    MouseWheel = SDL_MOUSEWHEEL,
    KeyPress = SDL_KEYDOWN,
    TimerEvent = SDL_USEREVENT,
};
#define GET_X(a) ((a).type == SDL_MOUSEMOTION?(a).motion.x:\
                  ((a).type == SDL_MOUSEBUTTONUP || (a).type == SDL_MOUSEBUTTONDOWN)?(a).button.x:\
                  (a).type == SDL_MOUSEWHEEL?(a).wheel.x:(abort(), 0))
#define GET_Y(a) ((a).type == SDL_MOUSEMOTION?(a).motion.y:\
((a).type == SDL_MOUSEBUTTONUP || (a).type == SDL_MOUSEBUTTONDOWN)?(a).button.y:\
(a).type == SDL_MOUSEWHEEL?(a).wheel.y:(abort(), 0))
#define GET_KEYSYM(a) ((a).key.keysym.sym)
typedef enum GuiKeySym {
    rightKey = SDLK_RIGHT,
    enter = SDLK_RETURN,
    leftKey = SDLK_LEFT,
    backspace = SDLK_BACKSPACE,
    delete = SDLK_DELETE
} GuiKeySym;
typedef SDL_Texture Image;
//#define IMAGE_WIDTH w
//#define IMAGE_HEIGHT h
#define IMAGE_SIZE(a) ({Size s; SDL_QueryTexture(a, NULL, NULL, &s.w, &s.h); s;})
typedef struct Painter {
    SDL_Renderer* gc;
    SDL_Surface* drawable;
    SDL_Window* window;
} Painter;
#else
#define TimerEvent LASTEvent+1
#include <X11/Xlib.h>
#define XK_LATIN1
#define XK_MISCELLANY
#include <X11/keysymdef.h>
typedef Window GuiWindow;
typedef struct Painter {
    Drawable drawable;
    GC gc;
} Painter;
enum {
    MotionEvent = MotionNotify,
};
typedef XEvent Event;
#define GET_X(a) ((a).xbutton.x)
#define GET_Y(a) ((a).xbutton.y)
//#define IS_BUTTON_RELEASE(a) ((a).type == ButtonRelease)
//#define IS_BUTTON_PRESS(a) ((a).type == ButtonPress)
//#define IS_KEY_PRESS(a) ((a).type == KeyPress)
//#define IS_MOTION(a) ((a).type == MotionNotify)
//#define IS_TIMER(a) ((a).type == TimerEvent)
#define GET_KEYSYM(a) (XLookupKeysym(&a.xkey, 0))
typedef enum GuiKeySym {
    right = XK_Right,
    enter = XK_Return,
    left = XK_Left,
    backspace = XK_BackSpace
} GuiKeySym;
#ifdef EXPOSE_X11_GLOBALS_PLEASE
extern Display* xdisplay;
extern int xDepth;
#endif
typedef XImage Image;
#define IMAGE_WIDTH width
#define IMAGE_HEIGHT height
#endif


//typedef struct Painter Painter;
typedef struct Size Size;

GuiWindow guiMakeWindow();
Painter guiMakePainter(GuiWindow w);
void guiMoveResizeWindow(GuiWindow win, int x, int y, int w, int h);
void guiShowWindow(GuiWindow w);
void guiHideWindow(GuiWindow w);
void guiClearWindow(GuiWindow w);
void guiGetTextPart(char* text, int wid, int* resWid, int* resCount);
struct Rect;
void guiSetClipRect(Painter* p, struct Rect r);
void guiUnsetClipRect(Painter* p);
#endif // XBACKEND_H
