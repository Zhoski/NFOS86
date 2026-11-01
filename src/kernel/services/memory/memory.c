#include <kernel/memory.h>

// ИСПРАВИТЬ КУЧУ
/* ---------- Куча ---------- */
#define HEAP_BASE           0x050000                // 2Мб
#define HEAP_SIZE           0x050000                // 1Мб

#define FREE                 1
#define BUSY                 0
#define NULL                 0
#define MEMORY_NOT_FOUND  0xFF

typedef struct {
    u32 size;
    u8 is_free;
    struct Heap* next;
} Heap;

Heap* block_heap = (Heap*)HEAP_BASE;

void create_heap() {
    block_heap->is_free = FREE;
    block_heap->size = HEAP_SIZE;
    block_heap->next = NULL;
}

#include <drivers/video/video.h>

void* malloc(u32 size) {
    u32 current_address = HEAP_BASE + sizeof(Heap) + size;
    Heap* current = block_heap;
    
    while (current != NULL)
    {
        u32 heap_end_address = HEAP_BASE + HEAP_SIZE;

        if (current_address + sizeof(Heap) >= heap_end_address) {
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
            u8* dest = (u8*)current_address;
            for (int i = 0; i < 12; i++) {
                dest[i] = t[i];
            }

            current->next = (struct Heap*)current_address;

            memset((void*)(current_address - size), 0, size);
            
            return (void*)(current_address - size);
        }

        current_address += (current->size + sizeof(Heap));
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