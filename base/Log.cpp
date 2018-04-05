//
// Created by huangkun on 30/03/2018.
//

#include "Log.h"
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <string.h>

static const int MAX_FORMAT_LENGTH = 4 * 1024;

void log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

std::string formatString(const char *format, ...) {
    char buff[MAX_FORMAT_LENGTH];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);
    return buff;
}

