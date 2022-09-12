
struct _XImage;
struct _XImage *loadImageZT(char* firstPartOfThePath,
                            char* secondPartOfThePath);
static struct _XImage *loadLocalImageZT(char* path) {
    return loadImageZT(MY_PATH, path);
}

void saveImageSomewhereNewWrongChannelsZT(struct _XImage *, char* name);

