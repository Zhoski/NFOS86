#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <kernel/process.h>
#include <lib/int.h>

typedef struct Scheduler Scheduler;

struct Scheduler {
  Process* process;
  u32 esp;
  u32 eip;
  Scheduler* next;
};

void scheduler_add_task(Process* process);

extern Scheduler sceduler_tbl[MAX_PROCESS];

#endif
