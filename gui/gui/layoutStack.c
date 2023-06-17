#include "layoutStack.h"
#include <stdio.h>
#include <stdlib.h>
Point defaultGetPos(LayoutVT** v) {
    fprintf(stderr, "getPos not set");
    myAbort( "getPos not set");
}
void defaultFeedbackSize(LayoutVT** v,Size s) {
    (void)s;
}
Size defaultAvailableSize(LayoutVT** v) {
    fprintf(stderr, "availableSize not set");
    myAbort("availableSize not set");
}
static LayoutVT defaultLayoutVt = {
    defaultGetPos,
    defaultFeedbackSize,
    defaultAvailableSize
};
static LayoutVT* defaultLayout = &defaultLayoutVt;
LayoutVT** stack[10] = {&defaultLayout};
size_t stackTop = 1;


Point exactPos(struct LayoutVT** lv) {
//    (void)lv;
    ExactLayout* l = (ExactLayout* )lv;
    return l->exactPos;
}
LayoutVT exactLayoutVT = {
    exactPos,
    defaultFeedbackSize,
    defaultAvailableSize
};




void* topLayout()
{
    ASSERT(stackTop > 0, "stack is empty, trying to get top")
    return stack[stackTop-1];
}

void pushLayout(void *g)
{
    ASSERT(stackTop < ELEMS(stack), "pushing layout, stackTop is %lld, elems is %llud, "
           "cond is %d, !cond id %d", stackTop, ELEMS(stack),
           stackTop < ELEMS(stack), !(stackTop < ELEMS(stack)))
    stack[stackTop] = g;
    stackTop++;
}

void popLayout()
{
    ASSERT(stackTop > 0, "stack is empty, trying to pop")
    stack[stackTop] = NULL;
    stackTop--;
}

ExactLayout makeExactLayout(Point pos) {
    return (ExactLayout){&exactLayoutVT, pos};
}
