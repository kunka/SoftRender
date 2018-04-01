//
// Created by huangkun on 30/03/2018.
//

#include "Log.h"
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <string.h>

static const int MAX_LOG_LENGTH = 16 * 1024;

static void _log(const char *format, va_list args) {
    int bufferSize = MAX_LOG_LENGTH;
    char *buf = nullptr;

    do {
        buf = new(std::nothrow) char[bufferSize];
        if (buf == nullptr)
            return; // not enough memory

        int ret = vsnprintf(buf, bufferSize - 3, format, args);
        if (ret < 0) {
            bufferSize *= 2;

            delete[] buf;
        } else
            break;

    } while (true);

    strcat(buf, "\n");

    fprintf(stdout, "%s", buf);
    fflush(stdout);
    delete[] buf;
}

void log(const char *str) {
    printf("%s\n", str);
}

void logf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    _log(format, args);
    va_end(args);
}