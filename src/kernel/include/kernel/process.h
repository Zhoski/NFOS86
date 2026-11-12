#ifndef PROCESS_H
#define PROCESS_H

#include <lib/int.h>

#define MAX_PROCESS    32

typedef enum {
  ACTIVE,
  SLEEP,
  DIED,
} State;

typedef struct {
  u32 pid;
  u32 ldt;
  u8*   name;
  void* stack;
  void* eip;
  void* heap_border;
  void* base;
  u32 limit;
  State state;
} Process;

void process_mk_tbl();
u32 process_spawn(u8* f);

extern Process* process_tbl;
extern u32 cur_pid;

#endif
