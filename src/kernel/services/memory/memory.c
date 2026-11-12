#include <kernel/memory.h>
#include <kernel/pmm.h>
#include <lib/debug.h>

#define FREE                 1
#define BUSY                 0
#define NULL                 0
#define MEMORY_NOT_FOUND  0xFF

typedef struct {
    u32 size;
    u8 is_free;
    struct Heap* next;
} Heap;

Heap* block_heap;

void* g_heap_base;
u32   g_heap_sz;

void create_heap(u32 base, u32 len) {
    //g_heap_base = pg_alloc(1);
    //for(u32 i = 1; i < pg_c; i++) {
    //    pg_alloc(1);
    //}

    block_heap = (Heap*)base;
    g_heap_sz = len;
    g_heap_base = (void*)base;

    block_heap->is_free = FREE;
    block_heap->size = g_heap_sz;
    block_heap->next = NULL;
    block_heap = (Heap*)base;
}

void* malloc(u32 size) {
    while(size%4!=0) size++;
    //video->kprintf("%x %x\n", g_heap_base, g_heap_sz);
    u32 current_addres = (u32)g_heap_base + sizeof(Heap) + size;
    //current_addres += current_addres % 4;
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
}

/**
 * Работает на костылях, исправить
 */
void free(u8* ptr) {
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

void memset(void* in, u8 v, u32 c) {
    asm volatile (
        "movl %[v], %%eax\n"
        "movl %[in], %%edi\n"
        "movl %[count], %%ecx\n"
        "rep stosb"
        :
        : [in] "r" ((u8*)in), [v] "r" ((u32)v), [count] "r" (c)
        : "eax", "edi", "ecx", "memory"
    );
}

void memcpy(void* __restrict__ from,void* __restrict__ in, u32 size) {
    asm volatile (
        "movl %[from], %%esi\n"
        "movl %[in],   %%edi\n"
        "movl %[count],%%ecx\n"
        "rep movsb"
        :
        : [from] "r" ((u8*)from), [in] "r" ((u8*)in), [count] "r" (size)
        : "esi", "edi", "ecx","memory"
    );
}
