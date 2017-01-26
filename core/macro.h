#pragma once

#define CORE_SLICE(x, y) x##y
#define CORE_STR_INDIRECTION(x) #x
#define CORE_CONVERT_TO_STR(x) CORE_STR_INDIRECTION(x)
