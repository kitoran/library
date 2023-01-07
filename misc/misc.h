#ifndef MISC_H
#define MISC_H

typedef unsigned int u32;
typedef unsigned char u8;
typedef int i32;
typedef long long int i64;
typedef long long unsigned int u64;
typedef short int i16;
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define CLAMP(a,min,max) (MIN(max, MAX(min, a)))
#define ELEMS(a) (sizeof(a)/sizeof(*(a)))
#define FOR_STATIC_ARRAY(type, element, array) for(type element = array; element < array+ELEMS(array); element++)
#define FOR(index, iters) for(int index = 0; index < iters; index++)
//#define FOR_STB_ARRAY(element, array) for(typeof(array) element = array; element < array+arrlen(array); element++)

#define DEBUG_PRINT(a, f) fprintf(stderr, #a " = " f "\n", (a));
//#ifndef tau
//#define tau 6.283185307179586
//#endif
#define STATIC(a,b,c) static a b; { static _Bool init = 0;\
    if(!init) {init = 1; b = (c);} }

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
#define ASSERT(a, msg, ...) if(!(a)) { \
    fprintf(stderr, "%s: " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__); \
    abort(); \
}
#define ABORT(message) { fprintf(stderr, "%s %s:%d\n function %s", message, __FILE__, __LINE__, __func__); abort(); }
//#define DEFER_MERGE(a,b) a##b
//#define DEFER_VARNAME(a) DEFER_MERGE(defer_scopevar_, a)
//#define DEFER_FUNCNAME(a) DEFER_MERGE(defer_scopefunc_, a)
//#define DEFER(BLOCK) void DEFER_FUNCNAME(__LINE__)(int *a) BLOCK; __attribute__((cleanup(DEFER_FUNCNAME(__LINE__)))) int DEFER_VARNAME(__LINE__)


#define STR_IMPL(a) #a
#define STR(a) STR_IMPL(a)

#endif // MISC_H
