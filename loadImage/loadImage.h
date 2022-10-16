


#ifdef SDL
struct SDL_Surface;
#define IMAGE struct SDL_Surface
#else
struct _XImage;
#define IMAGE struct _XImage
#endif

IMAGE *loadImageZT(char* firstPartOfThePath,
                            const char* secondPartOfThePath);
static IMAGE *loadLocalImageZT(const char* path) {
    return loadImageZT(MY_PATH, path);
}

void saveImageSomewhereNewWrongChannelsZT(IMAGE *, char* name);
#undef IMAGE
