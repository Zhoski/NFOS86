#include <kernel/process.h>
#include <cpu/gdt.h>
#include <kernel/memory.h>
#include <fs/fs.h>
#include <drivers/video/video.h>
#include <lib/string.h>
#include <lib/align.h>

#define USR_RW_P   0x07
#define USR_RW     0x06
#define KRN_RW_P   0x03

#define FREE       0xFFFF

//extern void jmp_usrmode(u32 entry, u32 stack);

Process prcs_tbl[MAX_PROCESS];
u32 cur_prcs = 0;
u32 prcs_start_executing = 0;

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

void execute_next_process() {
    /*while(prcs_tbl[cur_prcs].pid == 0xFFFF) {
      cur_prcs++;
      if(cur_prcs >= MAX_PROCESS) cur_prcs = 0;
    }*/

    lldt(prcs_tbl[cur_prcs].ldt_sel);

    /*for(;;);

    asm volatile(
      "pushl %0\n"
      "pushl %1\n"
      "pushl %2\n"
      "pushl %3\n"
      "pushl %4\n"
      "pushl %5\n"
      :
      : "r" (prcs_tbl[cur_prcs].regs.eax),
        "r" (prcs_tbl[cur_prcs].regs.edx),
        "r" (prcs_tbl[cur_prcs].regs.ecx),
        "r" (prcs_tbl[cur_prcs].regs.ebx),
        "r" (prcs_tbl[cur_prcs].regs.esi),
        "r" (prcs_tbl[cur_prcs].regs.edi)
      : "memory"
    );*/
    
    //jmp_usrmode(prcs_tbl[cur_prcs].regs.eip, prcs_tbl[cur_prcs].regs.esp);
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
  u32 limit = (64 << 10)-1;
  u32 base = (u32)malloc(limit);
  u32 stack_limit = (8 << 10)-1;
  process.heap_base = align(stack_limit + 1 + g_node.size, 1024, HIGHTER);
  process.stack_limit = stack_limit;
  process.limit = limit;
  process.base = base;
  process.state = ACTIVE;
  process.ldt_sel = ldt_mk_and_push(base, limit, stack_limit);
  process.regs.esp = stack_limit;
  process.regs.esp = align(process.regs.esp, 16, LOWER);
  process.regs.eip = align(process.stack_limit + 1, 1024, HIGHTER);

  tbl_add_process(&process);
  return &prcs_tbl[process.pid];
}
