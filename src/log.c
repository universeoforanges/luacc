#include <luacc/log.h>

#include <stdlib.h>
#include <stdio.h>

static bool_t g_verbose_logging_enabled = LUACC_FALSE;

void luacc_enable_verbose_logging(const bool_t should_enable)
{
	g_verbose_logging_enabled = should_enable;
}

bool_t luacc_has_verbose_logging(void)
{
	return g_verbose_logging_enabled;
}

void luacc_log(const luacc_log_level_t level, const char *msg)
{
	switch (level)
	{
	case LUACC_LOG_LEVEL_VERBOSE:
		if (!g_verbose_logging_enabled) break;

		printf("%s\n", msg);
		break;
	case LUACC_LOG_LEVEL_INFO:
		printf("info: %s\n", msg);
		break;
	case LUACC_LOG_LEVEL_WARNING:
		printf("warning: %s\n", msg);
		break;
	case LUACC_LOG_LEVEL_ERROR:
		fprintf(stderr, "error: %s\n", msg);
		break;
	case LUACC_LOG_LEVEL_FATAL:
		fprintf(stderr, "fatal: %s\n", msg);
		exit(1);
	default:
		char buf[33];
		snprintf(buf, sizeof(buf), "luacc_log(): invalid log level %i", level);

		luacc_log(LUACC_LOG_LEVEL_VERBOSE, buf);
	}
}
