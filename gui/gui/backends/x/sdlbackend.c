#include "backend.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_syswm.h>
#include <assert.h>
#include "combobox.h"
#include "stb_ds.h"
#include <stdio.h>
#include "gui.h"
#include <math.h>
#include "guiglobals.h"

extern char* appName;
extern int xDepth;
extern int maxDigitWidth, maxDigitHeight;
extern _Bool  redraw;
extern Rect rootWindowRect;
Event event;
_Atomic bool cursor;

TTF_Font* font;
TTF_Font* font_outline;
SDL_Window* rootWindow = 0;

void guiShowWindow(GuiWindow w) {
    SDL_ShowWindow(w);
}

void guiHideWindow(GuiWindow w) {
    SDL_HideWindow(w);
}
void guiRaiseWindow(GuiWindow w) {
    SDL_RaiseWindow(w);
}
void guiMoveResizeWindow
(GuiWindow win, int x,int y,int w,int h) {
    SDL_SetWindowSize(win,w,h);//SDL_absMoveResizeWindow(xdisplay,
    SDL_SetWindowPosition(win,x,y);
}
#include <windows.h>
GuiWindow guiMakeHiddenPopupWindow() {
    GuiWindow listWindow = SDL_CreateWindow("", 100, 100,
                                         700, 700, SDL_WINDOW_HIDDEN |SDL_WINDOW_BORDERLESS |
                                            SDL_WINDOW_SKIP_TASKBAR | /*SDL_WINDOW_UTILITY |*/
                                            SDL_WINDOW_POPUP_MENU /*| SDL_WINDOW_TOOLTIP*/);
//    XSelectInput(xdisplay, listWindow, ButtonReleaseMask);
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(listWindow, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;
    SetWindowLong(hwnd, GWL_EXSTYLE , WS_EX_TOOLWINDOW);
//    SetWindowLong(hwnd, GWL_STYLE ,  );
    return listWindow;
}
GuiWindow guiMakeWindow() {
    GuiWindow window = SDL_CreateWindow("", 100, 100,
                                         300, 400,
                                            SDL_WINDOW_SKIP_TASKBAR | SDL_WINDOW_RESIZABLE
                                            );
    return window;
}

void makeMenu() {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(rootWindow, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;


//    static HMENU hMenuBar;
    HMENU hMenuBar = CreateMenu();
    HMENU hEncoding =  CreateMenu();
    AppendMenuA(hMenuBar, MF_POPUP, (UINT_PTR)hEncoding, "Encoding");
    AppendMenuA(hEncoding, MF_STRING, 0x453434, "2 semitones");
//TODO: ask reaper about free ids
    SetMenu(hwnd, hMenuBar);
}

static SDL_Renderer** renderers = NULL;
Painter guiMakePainter(GuiWindow w) {
    Painter p = {SDL_CreateRenderer(w, -1,0),
                 SDL_GetWindowSurface(w),
                 w};
    arrpush(renderers,p.gc);
    return p;
}

_Bool guiSameWindow(Painter *p, _Bool def) {
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
    default: return def;
    }

}

void guiDrawLine(Painter *a, int b, int c, int d, int e)
{
    SDL_RenderDrawLine(a->gc, b, c, d, e);
}

void guiDrawRectangle(Painter *a, Rect r)
{
//    SDL_Rect r = {b,c,d,e};
    SDL_RenderDrawRect(a->gc, (SDL_Rect*)(&r));
}
void guiFillRectangle(Painter *a, Rect gr)
{
//    SDL_Rect r = gr;
//    fprintf(stderr, "filling rect (%d, %d) %dx%d\n", b, c, d,e);
    SDL_RenderFillRect(a->gc, (SDL_Rect*)(&gr));
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
#define OUTLINE_SIZE 1

typedef struct TextureHashEntry {
    char* key;
    SDL_Texture* value;
} TextureHashEntry;
/**** TODO: функции работы с текстом копируют строку на стек потому что SDL_TTF работает только с 0-терминированными строками
 * хотя ему приходится каждый раз вызывать strlen
 *
 */
Size guiDrawText(Painter* p, const char *text, int len, Point pos,
                 i32 color) {
    static TextureHashEntry* textureHash = NULL;

    SDL_Texture*textext = shget(textureHash, text);
    if(!textext) {
        static char copy[MAX_STRING_LEN];
        if (len >= MAX_STRING_LEN) ABORT("String too long");
        memcpy(copy, text, len);
        copy[len] = 0;
        if (copy[0] == '\0') return (Size) { 0, 0 };
        (void)len;
        SDL_Color sdlcolor = {
                (color >> 16)&0xff,
                (color >> 8)&0xff,
                (color >> 0)&0xff,
                255};
        SDL_Color black = {0,0,0,255};
        SDL_Color white = {255,255,255,255};
        SDL_Surface *bg_surface = TTF_RenderText_Blended(font_outline, copy, black);
        SDL_Surface *surfaceMessage =
                TTF_RenderUTF8_Blended(font, copy, white);

        SDL_Rect rrect = {OUTLINE_SIZE, OUTLINE_SIZE, surfaceMessage->w, surfaceMessage->h};
        SDL_SetSurfaceBlendMode(surfaceMessage, SDL_BLENDMODE_BLEND);
        SDL_BlitSurface(surfaceMessage, NULL, bg_surface, &rrect);

        textext = SDL_CreateTextureFromSurface
                        (p->gc, bg_surface);

        SDL_FreeSurface(surfaceMessage);
        SDL_FreeSurface(bg_surface);
        shput(textureHash, text, textext);
    }

    Size res; SDL_QueryTexture(textext, NULL, NULL, &res.w, &res.h);
    SDL_Rect rect = {
        pos.x,
        pos.y,
        res.w,
        res.h
    };
//    SDL_SetTextureBlendMode(textext, SDL_BLENDMODE_MOD);
    SDL_RenderCopy(p->gc, textext, 0, &rect);
//    SDL_DestroyTexture(textext);
//    SDL_FreeSurface(
///    SDL_BlitSurface(surfaceMessage, 0, p->drawable, &rect);
    return res;
}
Size guiTextExtents(/*Painter*p,*/const char*text, int len) {
    char copy[MAX_STRING_LEN];
    ASSERT(len < MAX_STRING_LEN, "sorry");
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
Size imageSize(Image* a) {
    Size s;
    SDL_QueryTexture(a, NULL, NULL, &s.w, &s.h);
    return s;
}

void guiDrawImage(Painter* p, IMAGE* i, int x, int y) {
//    SDL_Texture* t = SDL_CreateTextureFromSurface(p->gc, i);
    Size size = imageSize(i);
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
        SDL_GetTicks(),
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

void  guiStartDrawingEx(bool show) {
    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
        abort();
    }
//    int events = SDL_RegisterEvents(1);
//    assert(events == SDL_USEREVENT);
    SDL_AddTimer(BLINK_TIME, timerCallback, NULL);
    TimerEvent = SDL_RegisterEvents(2);
    RedrawEvent = TimerEvent+1;
    ASSERT(TTF_Init()==0, "can't initialize ttf library");
    // TODO: remova all absolute paths and preferably all path to resources
#ifndef _MSC_VER
    font = TTF_OpenFont("/home/n/.fonts/comici.ttf", 24);
#else
    font = TTF_OpenFont("C:/src/calibri.ttf", 20);

    font_outline = TTF_OpenFont("C:/src/calibri.ttf", 20);
    TTF_SetFontOutline(font_outline, OUTLINE_SIZE);
#endif
    for(char d[2] = {'0', '\x00'}; d[0] < '9'; d[0]++) {
        int w,h;
        TTF_SizeUTF8(font, d, &w, &h);
        maxDigitWidth = MAX(maxDigitWidth, w);
        maxDigitHeight = MAX(maxDigitHeight, h);
    }

    rootWindow = SDL_CreateWindow(
       appName,
       rootWindowRect.x, rootWindowRect.y, rootWindowRect.w, rootWindowRect.h,
//       0, 0,
       (show?SDL_WINDOW_SHOWN:SDL_WINDOW_HIDDEN) | SDL_WINDOW_RESIZABLE
   );

//    SDL_Rect f = {0,0,windowWidth, windowHeight};
//    SDL_RenderFillRect(renderer, &f);
    rootWindowPainter = guiMakePainter(rootWindow);
}
int RedrawEvent;
int TimerEvent;

void guiNextEvent(/*bool dontblock*/)
{
    FOR_STB_ARRAY(SDL_Renderer**, renderer, renderers) {
        SDL_RenderPresent(*renderer);
    }
//    if(redraw) {
//        redraw = false;
//        event.type = Expose;
//        return;
//    }
    int res ;
    if(/*dontblock*/false || redraw) {
        res = SDL_PollEvent(&event);
    } else {
        res = SDL_WaitEvent(&event);
        assert(res);
    }
    redraw = false;
    if(!res) {
        event.user = (SDL_UserEvent){ .type = RedrawEvent, .timestamp = SDL_GetTicks() };
    }
//    while(event.type == SDL_KEYMAPCHANGED) {
//        res = SDL_WaitEvent(&event); // for some reason sdl spams leymapchanged events
//    }
    while(guiSameWindow(&dropDownPainter, false)) {
        proccessComboBox();
        SDL_WaitEvent(&event);
    }
    if(event.type == SDL_WINDOWEVENT) {
        if(event.window.event == SDL_WINDOWEVENT_RESIZED  || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            fprintf(stderr,"resizeevent" );
            rootWindowRect.w = event.window.data1;
            rootWindowRect.h = event.window.data2;
        }
        if(event.window.event == SDL_WINDOWEVENT_MOVED) {
            fprintf(stderr,"moveevent" );
            rootWindowRect.x = event.window.data1;
            rootWindowRect.y = event.window.data2;
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
    SDL_Rect rect = {r.x, r.y, r.w, r.h};
    SDL_RenderSetClipRect(p->gc, &rect);
}

void guiUnsetClipRect(Painter* p) {
//    SDL_RenderPresent(p->drawable);
    SDL_RenderSetClipRect(p->gc, NULL);
}
void guiMoveWindow(GuiWindow window, int x, int y) {
    SDL_SetWindowPosition(window, x, y);
    rootWindowRect.pos = (Point){x,y};
}
