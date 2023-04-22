#include "linelayout.h"
#include "misc.h"
#include <stdio.h>
//#include <ASSERT.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void lineFeedbackSize(LineLayout* ll, Size size) {
    ll->across = MAX(ll->across, ll->horizontal?size.h:size.w);
    ll->filled += (ll->horizontal?size.w:size.h) + ll->spacing;
}

static Point lineGetPos(LineLayout* ll) {
    if(ll->horizontal) {
        return (Point){ll->pos.x+ll->filled, ll->pos.y};
    } else {
        return (Point){ll->pos.x, ll->pos.y+ll->filled};
    }
}

static const LayoutVT lineVT = {
    (Point (*)(struct LayoutVT**))lineGetPos,
    (void (*)(struct LayoutVT**,Size))lineFeedbackSize,
    NULL
};



LineLayout makeHorizontalLayout(int spacing)
{
    LineLayout ll = {.vt = &lineVT,
                     .horizontal = true,
                     .filled = 0,
                     .across = 0,
                     .spacing = spacing,
                     .pos = {0,0}};
    return ll;
}



LineLayout makeVerticalLayout(int spacing)
{
    LineLayout ll = makeHorizontalLayout(spacing);
    ll.horizontal = false;
    return ll;
}

Size getLineSize(LineLayout * ll)
{
    int along = MAX(0, ll->filled-ll->spacing);
    if(ll->horizontal) {
        return (Size){along, ll->across};
    } else {
        return (Size){ll->across, along};
    }
}
