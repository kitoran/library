#ifdef SDL
struct SDL_Texture;
#define IMAGE struct SDL_Texture
#else
struct _XImage;
#define IMAGE struct _XImage
#endif

IMAGE *loadImageZT(char* firstPartOfThePath,
                            const char* secondPartOfThePath);
IMAGE *loadLocalImageZT(const char* path);

void saveImageSomewhereNewWrongChannelsZT(IMAGE *, char* name);
//#undef IMAGE
