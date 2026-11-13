#include <slib/memory.h>
#include <slib/stdio.h>

#define FREE                 1
#define BUSY                 0
#define NULL                 0
#define MEMORY_NOT_FOUND  0xFF

typedef unsigned int u32;
typedef unsigned char u8;

typedef struct {
    u32 size;
    u8 is_free;
    struct Heap* next;
} Heap;

Heap* block_heap;
void* g_heap_base;
unsigned int g_heap_sz;

void memset(void* in, unsigned char v, unsigned int c) {
    asm volatile (
        "movl %[v], %%eax\n"
        "movl %[in], %%edi\n"
        "movl %[count], %%ecx\n"
        "rep stosb"
        :
        : [in] "r" ((unsigned char*)in), [v] "r" ((unsigned int)v), [count] "r" (c)
        : "eax", "edi", "ecx", "memory"
    );
}

void memcpy(void* src,void* dest, unsigned int n) {
    asm volatile (
        "movl %[from], %%esi\n"
        "movl %[in],   %%edi\n"
        "movl %[count],%%ecx\n"
        "rep movsb"
        :
        : [from] "r" ((unsigned char*)src), [in] "r" ((unsigned char*)dest), [count] "r" (n)
        : "esi", "edi", "ecx","memory"
    ); 
}

void init_heap() {
    asm volatile(
        "int $0x40\n"
        : "=a" (g_heap_base)
        : "a" (17)
        : "memory"
    );
  
    block_heap = (Heap*)g_heap_base;
    g_heap_sz = 32 * 1024;
    g_heap_base = (void*)g_heap_base;

    block_heap->is_free = FREE;
    block_heap->size = g_heap_sz;
    block_heap->next = NULL;
    block_heap = (Heap*)g_heap_base;
}

void* malloc(unsigned int size) {
    unsigned int ret;
    /*asm volatile(
        "int $0x40"
        :  "=a" (ret)
        : "a" (3), "b" (n)
        : "memory"
    );*/            

    while(size%4!=0) size++;
        u32 current_addres = (u32)g_heap_base + sizeof(Heap) + size;
        Heap* current = block_heap;
    
        while (current != NULL)
        {
            u32 heap_end_addres = (u32)g_heap_base + g_heap_sz;

            if (current_addres + sizeof(Heap) >= heap_end_addres) {
                return 0;
            }

            if (current->size >= (size + sizeof(Heap)) && current->is_free == FREE) {
                Heap new_heap = {
                    .is_free = FREE,
                    .size = current->size - size - sizeof(Heap),
                };
    
                current->size = size;
                current->is_free = BUSY;
    
                if (current->next != NULL) {
                    new_heap.next = current->next;
                }
                else {
                    new_heap.next = NULL;
                }           
    
                u8* t = (u8*)&new_heap;
                u8* dest = (u8*)current_addres;
                for (int i = 0; i < 12; i++) {
                    dest[i] = t[i];
                }
    
                current->next = (struct Heap*)current_addres;
    
                memset((void*)(current_addres - size), 0, size);
                
                return (void*)(current_addres - size);
            }
    
            current_addres += (current->size + sizeof(Heap));
            current = (Heap*)current->next;
    }
    
    return 0;    
    //return (void*)ret;
}

void free(void* ptr) {
    /*asm volatile(
        "int $0x40"
        :
        : "a" (4), "b" (ptr)
        : "memory"
    );*/

    
    u8* heap = ptr - sizeof(Heap);
    u8* next = heap;

    heap[4] = FREE;
    
    next += *(u32*)heap + sizeof(Heap);

    while (next[4] == FREE) {
        u32 new_size = *(u32*)next + *(u32*)heap + sizeof(Heap);
        u8* t = (u8*)&new_size;
        for (u32 i = 0;i < 4;i++) {
            heap[i] = t[i];
        }
         
        for (u32 i = 0;i < 4;i++) {
            heap[8 + i] = next[8 + i];
        }

        next += *(u32*)next + sizeof(Heap);
    }

    ptr = NULL;
}

unsigned int memcmp(void* p1, void* p2, unsigned int n) {
    unsigned int i = 0;
    while (i < n)
    {
        if(*(unsigned char*)(p1 + i) != *(unsigned char*)(p2 + i)) {
            return 0;
        }
        i++;
    }

    return 1;
}
