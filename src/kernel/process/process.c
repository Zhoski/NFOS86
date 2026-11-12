#include <kernel/process.h>
#include <cpu/gdt.h>
#include <kernel/memory.h>
#include <fs/fs.h>
#include <drivers/video/video.h>
#include <lib/string.h>

#define USR_RW_P   0x07
#define USR_RW     0x06
#define KRN_RW_P   0x03

Process* process_tbl;
u32 cur_pid = 0;

void process_mk_tbl() {
  process_tbl = malloc(sizeof(Process) * MAX_PROCESS);
  for(u32 i = 0; i < MAX_PROCESS; i++) process_tbl[i].pid = 0xFFFFFFFF;
}

void tbl_add_process(Process* process) {
  //video->kprintf("pr: %s\n", process->name);
  for(u32 i = 0; i < MAX_PROCESS; i++) {
    if(process_tbl[i].pid == 0xFFFFFFFF) {
      process->pid = i;
      memcpy(process, process_tbl + i, sizeof(Process));
      //process_tbl[i].pid = i;
      //video->kprintf("%s coping %x\n", process->name,process_tbl + i);
      //memcpy(process, process_tbl + i, sizeof(Process));
      //Process* test = process_tbl + i;
      //video->kprintf("%s\n", test->name);
      /*if(i==1) {
        //video->kprintf("%s\n",process_tbl[0].name);
      }*/
      break;
    }
  }
}

void tbl_rm_process(u32 pid) {
  process_tbl[pid].pid = 0xFFFFFFF;
}
/*
 * Возвращает pid процесса в случаи успеха и 0xFFFFFFFF в ином
*/
u32 process_spawn(u8* f) {
  Process process;

  u32 st = fs->check(f);
  if(!st) {
    return 0xFFFFFFFF;
  }

  u32 pname_len = strlen(g_node.name);
  process.name = malloc(pname_len + 1);
  memcpy(g_node.name, process.name, pname_len);
  //video->kprintf("Opa: %s\n", process.name);
  process.state = ACTIVE;
  process.eip = malloc(64 * 1024);
  process.base = process.eip;
  process.limit = 64 * 1024;
  process.stack = process.eip + 0x2000 + g_node.size + (g_node.size % 4);
  process.heap_border = process.stack + 4;
  process.ldt = ldt_mk_and_push((u32)process.eip, 64 * 1024);

  tbl_add_process(&process);
  return process.pid;
}
