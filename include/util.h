#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef int8_t byte_t;
typedef unsigned char bool_t; // 0: false, 1: true
#define TRUE 1
#define FALSE 0

typedef struct result
{
    int code;
    const char *msg;
} result_t;
// res: the result_t to check if it is failed
#define IS_FAIL(res) res.code != 0

result_t success_res(void);

#endif // UTIL_H