#include <kernel/process.h>
#include <cpu/gdt.h>
#include <kernel/memory.h>
#include <fs/fs.h>
#include <drivers/video/video.h>
#include <lib/string.h>

#define USR_RW_P   0x07
#define USR_RW     0x06
#define KRN_RW_P   0x03

#define FREE       0xFFFF

Process prcs_tbl[MAX_PROCESS];
u32 cur_prcs;

void prcs_tbl_init() {
  for(u32 i = 0; i < MAX_PROCESS; i++) {
    prcs_tbl[i].pid = FREE;
  }
}

void tbl_add_process(Process* process) {
  for(u32 i = 0; i < MAX_PROCESS; i++) {
    if(prcs_tbl[i].pid == FREE) {
      process->pid = i;
      memcpy(process, prcs_tbl + i, sizeof(Process));
      
      break;
    }
  }
}

void tbl_rm_process(u32 pid) {
  prcs_tbl[pid].pid = FREE;
}
/*
 * Возвращает указатель на процесс в prcs_tbl в случаи успеха и 0 в ином
*/
Process* process_spawn(u8* f) {
  u32 st = fs->check(f);
  if(!st) {
    return 0;
  }
  
  Process process;
  u32 limit = 64 << 10;
  u32 base = (u32)malloc(limit);
  process.limit = limit;
  process.base = base;
  process.state = ACTIVE;
  process.ldt_sel = ldt_mk_and_push(base, limit);

  tbl_add_process(&process);
  return &prcs_tbl[process.pid];
}
