#ifndef _LOG_H
#define _LOG_H

#include <luacc/util.h>

typedef enum
{
	LUACC_LOG_LEVEL_VERBOSE    = 0,    // will only be logged if verbose logging is enabled (e.g. by passing `-V` in the command line)
	LUACC_LOG_LEVEL_INFO       = 1,
	LUACC_LOG_LEVEL_WARNING    = 2,
	LUACC_LOG_LEVEL_ERROR      = 3,
	LUACC_LOG_LEVEL_FATAL      = 4,    // will exit the program with an exit code of 1 when logged
} luacc_log_level_t;

void luacc_log(const luacc_log_level_t level, const char *msg);

void luacc_enable_verbose_logging(const bool_t should_enable);
bool_t luacc_has_verbose_logging(void);

#endif // _LOG_H
