#include <stdlib.h>
#include <string.h>
#include "CuTest.h"

void TestDummy(CuTest *tc) {
	CuAssertStrEquals(tc, "dummy", "dummy");
}
