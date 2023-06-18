#ifdef SDL
struct SDL_Surface;
#define IMAGE struct SDL_Surface
#else
struct _XImage;
#define IMAGE struct _XImage
#endif

//IMAGE *loadImageZT(const char* path);
IMAGE *loadResourceImagePngZT(const char* resourceName);
//IMAGE *loadImageID(int id);
//IMAGE *loadLocalImageZT(const char* path);

void saveImageSomewhereNewWrongChannelsZT(IMAGE *, char* name);
//#undef IMAGE
