#ifndef MISC_H
#define MISC_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
typedef unsigned int u32;
typedef unsigned char u8;
typedef signed char i8;
typedef int i32;
typedef long long int i64;
typedef long long unsigned int u64;
typedef short int i16;
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define CLAMP(a,min,max) (MIN(max, MAX(min, a)))
#define SIGN(a) (((a) > 0) - ((a) < 0))
#define ELEMS(a) (sizeof(a)/sizeof(*(a)))
#define FOR_STATIC_ARRAY(type, element, array) for(type element = array; element < array+ELEMS(array); element++)
#define FOR(index, iters) for(int index = 0; index < iters; index++)
//#define FOR_STB_ARRAY(element, array) for(typeof(array) element = array; element < array+arrlen(array); element++)

#define DEBUG_PRINT(a, f) fprintf(stderr, #a " = " f "\n", (a));
//#ifndef tau
//#define tau 6.283185307179586
//#endif
#define STATIC(a,b,c) static a b; { static bool init = 0;\
    if(!init) {init = 1; b = (c);} }

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

extern bool aborted; // This should be an atomic bool. It's not my fault Justin Frankel used a compiler that doesn't provide it
void myAbort(char *message);
#define ASSERT(a, msg, ...) if(!(a)) { \
    char message[1000]; \
    snprintf(message, 1000, "%s:%d  %s: " msg "\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); \
    myAbort(message); \
}
#define QUICK_ASSERT(a) if(!(a)) { \
    char renderedmessage[1000]; \
    snprintf(renderedmessage, 1000, "%s:%d  %s: assertion \"%s\" failed\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, #a); \
    myAbort(renderedmessage); \
}

#define ABORT(message) { \
    char renderedmessage[1000]; \
    snprintf(renderedmessage, 1000, "%s %s:%d\n function %s", message, __FILE__, __LINE__, __func__); \
    myAbort(renderedmessage); \
}
//#define DEFER_MERGE(a,b) a##b
//#define DEFER_VARNAME(a) DEFER_MERGE(defer_scopevar_, a)
//#define DEFER_FUNCNAME(a) DEFER_MERGE(defer_scopefunc_, a)
//#define DEFER(BLOCK) void DEFER_FUNCNAME(__LINE__)(int *a) BLOCK; __attribute__((cleanup(DEFER_FUNCNAME(__LINE__)))) int DEFER_VARNAME(__LINE__)


#define STR_IMPL(a) #a
#define STR(a) STR_IMPL(a)

#ifdef __cplusplus
}
#endif

#endif // MISC_H
