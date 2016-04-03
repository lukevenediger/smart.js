/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_CS_DBG_H_
#define CS_COMMON_CS_DBG_H_

enum cs_log_level {
  LL_NONE = -1,
  LL_ERROR = 0,
  LL_WARN = 1,
  LL_INFO = 2,
  LL_DEBUG = 3,
  LL_VERBOSE_DEBUG = 4,

  _LL_MIN = -2,
  _LL_MAX = 5,
};

void cs_log_set_level(enum cs_log_level level);

#ifndef CS_DISABLE_STDIO

#include <stdio.h>

void cs_log_set_file(FILE *file);

extern enum cs_log_level cs_log_level;
void cs_log_print_prefix(const char *func);
void cs_log_printf(const char *fmt, ...);

#define LOG(l, x)                  \
  if (cs_log_level >= l) {         \
    cs_log_print_prefix(__func__); \
    cs_log_printf x;               \
  }

#ifndef CS_NDEBUG

#define DBG(x)                            \
  if (cs_log_level >= LL_VERBOSE_DEBUG) { \
    cs_log_print_prefix(__func__);        \
    cs_log_printf x;                      \
  }

#else /* NDEBUG */

#define DBG(x)

#endif

#else /* CS_DISABLE_STDIO */

#define LOG(l, x)
#define DBG(x)

#endif

#endif /* CS_COMMON_CS_DBG_H_ */
