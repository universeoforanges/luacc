#include "util.h"

result_t success_res(void)
{
    result_t res;
    res.code = 0;
    res.msg = "";
    return res;
}