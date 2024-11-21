#ifndef LOG_H_
#define LOG_H_
#include "printf.h"

#define dlog(s, ...) printf("[LOG]: " s "\n" __VA_OPT__(,) __VA_ARGS__)
//#define dlog(...) (void)0

#endif // LOG_H_
