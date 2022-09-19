


#ifdef SDL
struct SDL_Surface;
#define IMAGE struct SDL_Surface
#else
struct _XImage;
#define IMAGE struct _XImage
#endif

IMAGE *loadImageZT(char* firstPartOfThePath,
                            char* secondPartOfThePath);
static IMAGE *loadLocalImageZT(char* path) {
    return loadImageZT(MY_PATH, path);
}

void saveImageSomewhereNewWrongChannelsZT(IMAGE *, char* name);
#undef IMAGE
