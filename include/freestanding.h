#pragma once

#if (__STDC_HOSTED__ == 1)
  #error "Freestanding headers only available with -freestanding"
#endif

#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
