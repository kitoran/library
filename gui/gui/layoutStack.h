#ifndef LAYOUTSTACK_H
#define LAYOUTSTACK_H
#include "gui.h"

typedef struct LayoutVT {
    Point (*getPos)(struct LayoutVT*);
    void (*feedbackSize)(struct LayoutVT*,Size);
    Size (*availableSize)(struct LayoutVT*); // unused
    // probably better to use a
    // a function like Size (*getSize)(struct LayoutVT*,
     //                         enum sizePolicy policy);
} LayoutVT;

void pushLayout(void*);
void popLayout();
void* topLayout();



typedef struct ExactLayout {
    LayoutVT* vt;
    Point exactPos;
} ExactLayout;
ExactLayout makeExactLayout(Point pos);
#endif // LAYOUTSTACK_H
