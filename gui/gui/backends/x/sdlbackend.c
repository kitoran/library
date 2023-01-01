#include "backend.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include "gui.h"
#include <math.h>

extern char* appName;
extern int xDepth;
extern int maxDigitWidth, maxDigitHeight;
extern _Bool  redraw;
extern Size rootWindowSize;
Event event;
_Atomic bool cursor;

TTF_Font* font;
SDL_Window* rootWindow = 0;
SDL_Renderer* rootWindowRenderer;

void guiShowWindow(GuiWindow w) {
    SDL_ShowWindow(w);
}

void guiHideWindow(GuiWindow w) {
    SDL_HideWindow(w);
}

void guiMoveResizeWindow
(GuiWindow win, int x,int y,int w,int h) {
    SDL_SetWindowSize(win,w,h);//SDL_absMoveResizeWindow(xdisplay,
    SDL_SetWindowPosition(win,x,y);
}

GuiWindow guiMakeWindow() {
    GuiWindow listWindow = SDL_CreateWindow("", 0, 0,
                                         700, 700, 0);
//    XSelectInput(xdisplay, listWindow, ButtonReleaseMask);
    return listWindow;
}

Painter guiMakePainter(GuiWindow w) {
    Painter p = {SDL_CreateRenderer(w, -1,0),
                 SDL_GetWindowSurface(w),
                 w};
    return p;
}

_Bool guiSameWindow(Painter *p) {
    u32 wid = SDL_GetWindowID(p->window);
    switch(event.type) {
    case SDL_DROPBEGIN:
    case SDL_DROPCOMPLETE:
    case SDL_DROPTEXT:
    case SDL_DROPFILE:
        return wid == event.drop.windowID;
    case SDL_WINDOWEVENT:
        return wid == event.window.windowID;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        return wid == event.key.windowID;
    case SDL_TEXTINPUT:
        return wid == event.text.windowID;
    case SDL_TEXTEDITING:
        return wid == event.edit.windowID;
    case SDL_MOUSEMOTION:
        return wid == event.motion.windowID;
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
        return wid == event.edit.windowID;
    default: return true;
    }

}

void guiDrawLine(Painter *a, int b, int c, int d, int e)
{
    SDL_RenderDrawLine(a->gc, b, c, d, e);
}

void guiDrawRectangle(Painter *a, int b, int c, int d, int e)
{
    SDL_Rect r = {b,c,d,e};
    SDL_RenderDrawRect(a->gc, &r);
}
void guiFillRectangle(Painter *a, int b, int c, int d, int e)
{
    SDL_Rect r = {b,c,d,e};
//    fprintf(stderr, "filling rect (%d, %d) %dx%d\n", b, c, d,e);
    SDL_RenderFillRect(a->gc, &r);
}

void guiSetForeground(Painter *a, unsigned long b)
{
    SDL_SetRenderDrawColor(a->gc, (b >> 16) & 0xff,
                           (b >> 8) & 0xff,
            (b >> 0) & 0xff,
            (b >> 24) & 0xff);
}

//void guiDrawTextWithLen(Painter *a, int b, int c, char *d, unsigned long e)
//{
//    Xutf8DrawString(xdisplay, a->drawable, xFontSet, a->gc, b, c, d, e);
//}

void guiSetSize(u32 w, u32 h)
{
    SDL_SetWindowSize(rootWindow, w, h);
}
//void guiMoveResizeWindow(GuiWindow win, int x, int y, int w, int h) {
//    XMoveResizeWindow(xdisplay,
//                      win,
//                      x,y, h, w);
//}
void guiClearWindow(GuiWindow w) {
    SDL_RenderClear(SDL_GetRenderer(w));
}
#warning remove leak


/**** TODO: функции работы с текстом копируют строку на стек потому что SDL_TTF работает только с 0-терминированными строками
 * хотя ему приходится каждый раз вызывать strlen
 *
 * еще надо сделать хеш для текстур
 */
Size guiDrawText(Painter* p, const char *text, int len, Point pos,
                 i32 color) {
    char copy[len+1];
    memcpy(copy, text, len);
    copy[len] = 0;
    if(copy[0]=='\0') return STRU(Size, 0, 0);
    (void)len;
    SDL_Color sdlcolor = {
            (color >> 16)&0xff,
            (color >> 8)&0xff,
            (color >> 0)&0xff,
            255};
    SDL_Surface *surfaceMessage =
            TTF_RenderUTF8_Solid(font, copy, sdlcolor);
    SDL_Texture*textext = SDL_CreateTextureFromSurface
                    (p->gc, surfaceMessage);
    Size res = {surfaceMessage->w, surfaceMessage->h};
    SDL_FreeSurface(surfaceMessage);

    SDL_Rect rect = {
        pos.x,
        pos.y,
        res.w,
        res.h
    };
    SDL_RenderCopy(p->gc, textext, 0, &rect);


    SDL_DestroyTexture(textext);
//    SDL_FreeSurface(
///    SDL_BlitSurface(surfaceMessage, 0, p->drawable, &rect);
    return res;
}
Size guiTextExtents(/*Painter*p,*/const char*text, int len) {
    char copy[len+1];
    memcpy(copy, text, len);
    copy[len] = 0;
    Size res ;
    TTF_SizeUTF8(font, copy, &res.w, &res.h);
    return res;
}
void guiGetTextPart(char* text, int wid, int* resWid, int* resCount) {
//    char copy[len+1];
//    memcpy(copy, text, len);
//    copy[len] = 0;
    TTF_MeasureUTF8(font, text, wid, resWid, resCount);
}

void guiDrawImageEx(Painter* p, IMAGE* i, int x, int y, Size* size) {
//    SDL_Texture* t = SDL_CreateTextureFromSurface(p->gc, i);
//    Size si = IMAGE_SIZE(i);
    SDL_Rect rect = {x, y, size->w, size->h};
    SDL_RenderCopy(p->gc, i, NULL, &rect);//
//    SDL_BlitSurface(i, NULL, p->drawable, &rect);
//    SDL_UpdateWindowSurface( rootWindow );
}
void guiDrawImage(Painter* p, IMAGE* i, int x, int y) {
//    SDL_Texture* t = SDL_CreateTextureFromSurface(p->gc, i);
    Size size = IMAGE_SIZE(i);
    SDL_Rect rect = {x, y, size.w, size.h};
    SDL_RenderCopy(p->gc, i, NULL, &rect);//
//    SDL_BlitSurface(i, NULL, p->drawable, &rect);
//    SDL_UpdateWindowSurface( rootWindow );
}


#define BLINK_TIME 530
static Uint32 timerCallback(Uint32 interval, void *param) {
    (void) interval;
    (void)param;
    SDL_UserEvent e = {
        TimerEvent,
        0,
        0,
        0,
        0,
        0
    };
    SDL_Event event; event.user = e;
    SDL_PushEvent(&event);
    cursor = !cursor;
    return BLINK_TIME;
}
void guiStartDrawing(/*const char* appName*/) {
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
        abort();
    }
//    int events = SDL_RegisterEvents(1);
//    assert(events == SDL_USEREVENT);
    SDL_AddTimer(BLINK_TIME, timerCallback, NULL);

    TTF_Init();
    font = TTF_OpenFont("/home/n/.fonts/comici.ttf", 24);


    for(char d[2] = {'0', '\x00'}; d[0] < '9'; d[0]++) {
        int w,h;
        TTF_SizeUTF8(font, d, &w, &h);
        maxDigitWidth = MAX(maxDigitWidth, w);
        maxDigitHeight = MAX(maxDigitHeight, h);
    }

    rootWindow = SDL_CreateWindow(
       appName,
       700,0,700, 700,
//       0, 0,
       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
   );
    rootWindowRenderer = SDL_CreateRenderer(rootWindow, -1,  0);
//    SDL_Rect f = {0,0,windowWidth, windowHeight};
//    SDL_RenderFillRect(renderer, &f);
    rootWindowPainter = STRU(Painter,
                             rootWindowRenderer, SDL_GetWindowSurface(rootWindow), rootWindow);
    // Create a "Graphics Context"
//    GC gc = XCreateGC(xdisplay , rootWindow, 0, NULL);
//    xFontStruct = XLoadQueryFont(xdisplay, "fixed");
    // Tell the GC we draw using the white color
//    XSetForeground(xdisplay , gc, whiteColor);

//    XFlush(xdisplay);

}

void guiNextEvent()
{
    SDL_RenderPresent(rootWindowPainter.gc);
//    if(redraw) {
//        redraw = false;
//        event.type = Expose;
//        return;
//    }
    int res = SDL_WaitEvent(&event);
//    while(event.type == SDL_KEYMAPCHANGED) {
//        res = SDL_WaitEvent(&event); // for some reason sdl spams leymapchanged events
//    }
    assert(res);
    if(event.type == SDL_WINDOWEVENT) {
        if(event.window.event == SDL_WINDOWEVENT_RESIZED  || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            fprintf(stderr,"resizeevent" );
            rootWindowSize.w = event.window.data1;
            rootWindowSize.h = event.window.data2;
        }
        if(event.window.event == SDL_WINDOWEVENT_CLOSE) {
            return;
        }
        if(event.window.event == SDL_WINDOWEVENT_LEAVE && SDL_GetMouseState(0,0) != 0) {
            SDL_CaptureMouse(SDL_TRUE);
        }
    }
    // the next piece is supposedly not needed starting from SDL 2.0.22. My system has SDL 2.0.20
    // the behavior can be unexpected if the user holds two buttons and only releases one of them,
    // i will fix it when it bites me
//    if( event.type == SDL_MOUSEBUTTONDOWN) {
//    }
    if( event.type == SDL_MOUSEBUTTONUP) {
        SDL_CaptureMouse(SDL_FALSE);
    }

}
// doesn't stack rects for now
void guiSetClipRect(Painter* p, Rect r) {
    SDL_Rect rect = {r.x, r.y, r.width, r.height};
    SDL_RenderSetClipRect(p->gc, &rect);
}

void guiUnsetClipRect(Painter* p) {
//    SDL_RenderPresent(p->drawable);
    SDL_RenderSetClipRect(p->gc, NULL);
}
