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
  u32 eax; u32 ebx; u32 ecx; u32 edx; u32 esi; u32 edi;
  u32 esp; u32 eip; u32 ebp;
} Register;

typedef struct {
  u32 pid;
  u32 ldt_sel;
  u32 base;
  u32 limit;
  u32 stack_limit;
  u32 heap_base;
  Register regs;
  State state;
} Process;


void prcs_tbl_init();
Process* process_spawn(u8* f);

void execute_next_process();

extern Process prcs_tbl[MAX_PROCESS];
extern u32 cur_prcs;
extern u32 prcs_start_executing;

#endif
