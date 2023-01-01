//#include "gui.h"
extern int maxDigitWidth;
extern int maxDigitHeight;
extern bool redraw;
struct context {
    void* active;
    int pos;
#define MAX_INPUT_LEN 100
    char editedString[MAX_INPUT_LEN];
    char nullTerminator;
    int editedStringLen;
//    void (*commit)(void* data);
};
extern struct context context;
extern Size rootWindowSize;
