#include "gui.h"
#include "guiglobals.h"
int maxDigitWidth = 0;
int maxDigitHeight = 0;
bool redraw = false;
struct context context = {
    NULL,
    0,
    {0},
    0,
    0
};
//Rect rootWindowRect = {300, 20, 700, 700};

Painter rootWindowPainter;
