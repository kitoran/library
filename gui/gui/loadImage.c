#include "stb_image.h"
#include "gui.h"
#include "loadImage.h"
#include "newFile.h"
#include "stb_image_write.h"
#ifdef SDL
#include <SDL.h>
#else
#include <X11/Xlib.h>
#endif
#ifndef _MSC_VER
#include <linux/limits.h>
#else
#include <windows.h>
#define PATH_MAX _MAX_PATH
#endif

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
//#ifdef SDL
//struct SDL_Surface;
//#define IMAGE struct SDL_Surface
//#else
//struct _XImage;
//#define IMAGE struct _XImage
//#endif
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("Ofast")
#endif
#ifndef SDL
extern Display * xdisplay;
#endif
// i use strncat completely wrong, completely wrong
static IMAGE *common(int x, unsigned char *data, int y)
{
    for(int i = 0; i < x*y; i++) {
        char temp;
//        memcpy(temp, rcdata+i*4, 4);
        temp = data[i*4+3];
        data[i*4+3] = data[i*4];
        data[i*4] = temp;
    }
#ifdef SDL
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(data, x, y, 24, x*4, SDL_PIXELFORMAT_ARGB32);
    SDL_bool res = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
//    SDL_Texture * texture = SDL_CreateTextureFromSurface(rootWindowPainter.gc, surface);
    fprintf(stderr, "SDL_SetHint returned %d", res);

    return surface;
}

SDL_Surface * loadImageZT(const char *imagePath) {
    int x,y, n;
    unsigned char *data = stbi_load(imagePath, &x, &y, &n, 4);
    ASSERT(data, "couldn't load image %s", imagePath);
    SDL_Surface *surface = common(x, data, y);
#else
    XImage *res = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                     ZPixmap, 0, data, x, y, 32,
                             x*4);
#endif
    //IMG_LoadTexture_RW../?
    return surface;
}
SDL_Surface * loadResourceImagePngZT(char* resourceName) {
    HGLOBAL     res_handle = NULL;
    HRSRC       res;
    char *      res_data;
    DWORD       res_size;
    // NOTE: providing g_hInstance is important, NULL might not work
    HMODULE hModule = NULL;
    GetModuleHandleEx(
      GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
      (LPCTSTR)loadResourceImagePngZT,
      &hModule);
    res = FindResourceA(hModule, resourceName, MAKEINTRESOURCEA(10));
    res_handle = LoadResource(hModule, res);
    res_data = (char*)LockResource(res_handle);
    res_size = SizeofResource(hModule, res);


    int x,y, n;
    unsigned char *data = stbi_load_from_memory   (res_data,res_size, &x, &y, &n, 4);
    ASSERT(data, "couldn't load image %s", resourceName);
    SDL_Surface *surface = common(x, data, y);

    //IMG_LoadTexture_RW../?
    return surface;
}
#ifdef __GNUC__
#pragma GCC pop_options
#endif
FILE* saveImage = 0;

#ifndef SDL
static void stbicallback(void *context, void *data, int size) {
    (void)context;
    fwrite(data, size, 1, saveImage);
}
void saveImageSomewhereNewWrongChannelsZT(XImage *image, char *name) {
    for(int i = 0; i < image->width*image->height; i++) {
        ((char*)(image->data))[i*4+3] = 0xff;
        char t = ((char*)(image->data))[i*4+2];
        ((char*)(image->data))[i*4+2] = ((char*)(image->data))[i*4+0];
        ((char*)(image->data))[i*4+0] = t;
    }
    char* path = newFile(name, "bmp");
    saveImage = fopen(path, "w");
    stbi_write_bmp_to_func(stbicallback, NULL, image->width,
                           image->height, 4, image->data);
    fclose(saveImage);
    saveImage=0;
}
#endif

//IMAGE *loadLocalImageZT(const char *path) {
//    return loadImageZT(MY_PATH, path);
//}
