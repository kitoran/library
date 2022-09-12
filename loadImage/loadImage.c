#include "loadImage.h"
#include "stb_image.h"
#include "newFile.h"
#include "stb_image_write.h"
#include <X11/Xlib.h> // Every Xlib program must include this
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#pragma GCC push_options
#pragma GCC optimize ("Ofast")

extern Display * xdisplay;
XImage *loadImageZT(char* startOfPath, char *path) {

    char imagePath[PATH_MAX] = {};
    int r;
    r+=5;
//#define stringify2(a) #a
//#define stringify(a) stringify2(a)
    strncat(imagePath, startOfPath, PATH_MAX);
    strncat(imagePath, "/", PATH_MAX);
    strncat(imagePath, path, PATH_MAX);
    int x,y, n;
    unsigned char *data = stbi_load(imagePath, &x, &y, &n, 4);
    if(!data) abort();
    for(int i = 0; i < x*y; i++) {
        char temp;
//        memcpy(temp, rcdata+i*4, 4);
        temp = data[i*4+2];
        data[i*4+2] = data[i*4];
        data[i*4] = temp;
    }
    XImage *res = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                 ZPixmap, 0, data, x, y, 32,
                         x*4);

    return res;
}
#pragma GCC pop_options

FILE* saveImage = 0;
static void stbicallback(void *context, void *data, int size) {
    fwrite(data, size, 1, saveImage);
}
void saveImageSomewhereNewWrongChannelsZT(struct _XImage *image, char *name) {
    for(int i = 0; i < image->width*image->height; i++) {
        image->data[i*4+3] = 0xff;
    }
    char* path = newFile(name, "bmp");
    saveImage = fopen(path, "w");
    stbi_write_bmp_to_func(stbicallback, NULL, image->width,
                           image->height, 4, image->data);
    fclose(saveImage);
    saveImage=0;
}
