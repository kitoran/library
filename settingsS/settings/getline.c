/********************************************
BSD 2-Clause License

Copyright (c) 2022, Costantino Grana
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********************************************************************/
#include "getline.h"

#include <stdlib.h>
#include <errno.h>

// MSVC specific implementation
static void fseterr(FILE *fp)
{
    struct file { // Undocumented implementation detail
        unsigned char *_ptr;
        unsigned char *_base;
        int _cnt;
        int _flag;
        int _file;
        int _charbuf;
        int _bufsiz;
    };
    #define _IOERR 0x10

    ((struct file *)fp)->_flag |= _IOERR;
}

ssize_t getdelim(char **restrict lineptr, size_t *restrict n, int delim, FILE *restrict stream)
{
    if (lineptr == NULL || n == NULL || stream == NULL || (*lineptr == NULL && *n != 0)) {
        errno = EINVAL;
        return -1;
    }
    if (feof(stream) || ferror(stream)) {
        return -1;
    }

    if (*lineptr == NULL) {
        *n = 256;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            fseterr(stream);
            errno = ENOMEM;
            return -1;
        }
    }
    ssize_t nread = 0;
    int c = EOF;
    while (c != delim) {
        c = fgetc(stream);
        if (c == EOF) {
            break;
        }
        if (nread >= *n - 1) {
            size_t newn = *n * 2;
            char *newptr = realloc(*lineptr, newn);
            if (newptr == NULL) {
                fseterr(stream);
                errno = ENOMEM;
                return -1;
            }
            *lineptr = newptr;
            *n = newn;
        }
        (*lineptr)[nread++] = c;
    }
    if (c == EOF && nread == 0) {
        return -1;
    }
    (*lineptr)[nread] = 0;
    return nread;
}

ssize_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream)
{
    return getdelim(lineptr, n, '\n', stream);
}

