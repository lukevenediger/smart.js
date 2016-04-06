/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "cc3200_sj_hal.h"

#include <malloc.h>
#include <string.h>

#include "hw_types.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"

#include <common/platform.h>

#include "simplelink.h"
#include "device.h"
#include "oslib/osi.h"

#include "smartjs/src/sj_hal.h"
#include "smartjs/src/sj_v7_ext.h"
#include "v7/v7.h"

extern OsiMsgQ_t s_v7_q;

void sj_invoke_cb(struct v7 *v7, v7_val_t func, v7_val_t this_obj,
                  v7_val_t args) {
  struct sj_event e;
  struct v7_invoke_event_data *ied = calloc(1, sizeof(*ied));
  ied->func = func;
  ied->this_obj = this_obj;
  ied->args = args;
  v7_own(v7, &ied->func);
  v7_own(v7, &ied->this_obj);
  v7_own(v7, &ied->args);
  e.type = V7_INVOKE_EVENT;
  e.data = ied;
  osi_MsgQWrite(&s_v7_q, &e, OSI_WAIT_FOREVER);
}

/* Defined in linker script. */
extern unsigned long _heap;
extern unsigned long _eheap;

size_t sj_get_heap_size() {
  return ((char *) &_eheap - (char *) &_heap);
}

size_t sj_get_free_heap_size() {
  size_t avail = sj_get_heap_size();
  struct mallinfo mi = mallinfo();
  avail -= mi.arena;    /* Claimed by allocator. */
  avail += mi.fordblks; /* Free in the area claimed by allocator. */
  return avail;
}

size_t sj_get_min_free_heap_size() {
  /* Not supported */
  return 0;
}

size_t sj_get_fs_memory_usage() {
  return 0; /* Not even sure if it's possible to tell. */
}

void sj_wdt_feed() {
  /* TODO */
}

void sj_wdt_set_timeout(int secs) {
  /* TODO */
}

void sj_wdt_enable() {
  /* TODO */
}

void sj_wdt_disable() {
  /* TODO */
}

void sj_system_restart(int exit_code) {
  (void) exit_code;
  /* Turns out to be not that easy. In particular, using *Reset functions is
   * not a good idea.
   * https://e2e.ti.com/support/wireless_connectivity/f/968/p/424736/1516404
   * Instead, the recommended way is to enter hibernation with immediate wakeup.
   */
  sl_Stop(50 /* ms */);
  MAP_PRCMHibernateIntervalSet(328 /* 32KHz ticks, 100 ms */);
  MAP_PRCMHibernateWakeupSourceEnable(PRCM_HIB_SLOW_CLK_CTR);
  MAP_PRCMHibernateEnter();
}

void sj_usleep(int usecs) {
  osi_Sleep(usecs / 1000 /* ms */);
}

void mongoose_schedule_poll() {
  struct sj_event e;
  e.type = MG_POLL_EVENT;
  osi_MsgQWrite(&s_v7_q, &e, OSI_WAIT_FOREVER);
}
