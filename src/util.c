#include <luacc/util.h>

int luacc_get_bits(const int n, const int idx, const int size)
{
	const int SHIFTED = n >> idx;
	const int MASK = (1 << size) - 1;

	return SHIFTED & MASK;
}
