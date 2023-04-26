#include "gui.h"
extern int maxDigitWidth;
extern int maxDigitHeight;
extern bool redraw;
struct context {
    void* active;
    int pos;
#define MAX_STRING_LEN 100
    //TODO there are likely loads of off-by-1 bugs connected to this constant
    char editedString[MAX_STRING_LEN];
    char nullTerminator;
    int editedStringLen;
    int numberOfDropDownElements;
    Size sizeOfDropDownElements;
//    void (*commit)(void* data);
};
extern struct context context;
//extern Rect rootWindowRect;
