#include "layoutStack.h"
#include <stdio.h>
#include <stdlib.h>
Point defaultGetPos(LayoutVT* v) {
    fprintf(stderr, "getPos not set");
    abort();
}
void defaultFeedbackSize(LayoutVT* v,Size s) {
    (void)s;
}
Size defaultAvailableSize(LayoutVT* v) {
    fprintf(stderr, "availableSize not set");
    abort();
}
LayoutVT defaultLayout = {
    defaultGetPos,
    defaultFeedbackSize,
    defaultAvailableSize
};

void* stack[10] = {&defaultLayout};
size_t stackTop = 1;


Point exactPos(struct LayoutVT* lv) {
//    (void)lv;
    ExactLayout* l = (ExactLayout* )lv;
    return l->exactPos;
}
LayoutVT exactLayoutVT = {
    exactPos,
    defaultFeedbackSize,
    defaultAvailableSize
};




void *topLayout()
{
    ASSERT(stackTop > 0, "stack is empty, trying to get top")
    return stack[stackTop-1];
}

void pushLayout(void *g)
{
    ASSERT(stackTop < ELEMS(stack), "pushing grid, stackTop is %lld, elems is %llud, "
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
