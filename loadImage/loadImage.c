#include "stb_image.h"
#include "loadImage.h"
#include "newFile.h"
#include "stb_image_write.h"
#ifdef SDL
#include <SDL.h>
extern SDL_Renderer* rootWindowRenderer;
#else
#include <X11/Xlib.h>
#endif
#ifndef _MSC_VER
#include <linux/limits.h>
#else
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
IMAGE *loadImageZT(char* startOfPath, const char *path) {

    char imagePath[PATH_MAX] = {0};
//    int r;
//    r+=5;
//#define stringify2(a) #a
//#define stringify(a) stringify2(a)
    strncat(imagePath, startOfPath, PATH_MAX-1);
    strncat(imagePath, "/", PATH_MAX);
    strncat(imagePath, path, PATH_MAX-1);
    int x,y, n;
    unsigned char *data = stbi_load(imagePath, &x, &y, &n, 4);
    if(!data) abort();
    for(int i = 0; i < x*y; i++) {
        char temp;
//        memcpy(temp, rcdata+i*4, 4);
        temp = data[i*4+3];
        data[i*4+3] = data[i*4];
        data[i*4] = temp;
    }
#ifdef SDL
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(data, x, y, 24, x*4, SDL_PIXELFORMAT_ARGB32);
#else
    XImage *res = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                     ZPixmap, 0, data, x, y, 32,
                             x*4);
#endif
    SDL_Texture * texture = SDL_CreateTextureFromSurface(rootWindowRenderer, surface);
    //IMG_LoadTexture_RW../?
    SDL_FreeSurface(surface);
    return texture;
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

IMAGE *loadLocalImageZT(const char *path) {
    return loadImageZT(MY_PATH, path);
}
