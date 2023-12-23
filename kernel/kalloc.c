// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct spinlock stealing_lock;
  struct run *freelist;
} kmem[NCPU];

char* kmem_lock_names[] = {
  "kmem_cpu_0",   
  "kmem_cpu_1",  
  "kmem_cpu_2",  
  "kmem_cpu_3",  
  "kmem_cpu_4",  
  "kmem_cpu_5",  
  "kmem_cpu_6",  
  "kmem_cpu_7",
};
  

void
kinit()
{
  for(int i = 0;i < NCPU; ++i){
    char name[12] = {0};
    snprintf(name, 11, "stealing_%d", i);
    initlock(&kmem[i].stealing_lock, name);
    initlock(&kmem[i].lock, kmem_lock_names[i]);
  }
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;
  
  push_off();
  
  int cpu = cpuid();
  acquire(&kmem[cpu].lock);
  r->next = kmem[cpu].freelist;
  kmem[cpu].freelist = r;
  release(&kmem[cpu].lock);

  pop_off();
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
  
  push_off();
  
  int cpu = cpuid();

  acquire(&kmem[cpu].lock);
  
  if(!kmem[cpu].freelist) {
    acquire(&kmem[cpu].stealing_lock);
    release(&kmem[cpu].lock);
    int steal_left = 64;
    for(int i = 0; i < NCPU; ++i){
      int next_cpu = (cpu + i + 1) % NCPU;
      acquire(&kmem[next_cpu].lock);
      struct run* n_r = kmem[next_cpu].freelist;
      while(n_r && steal_left){
        kmem[next_cpu].freelist = n_r->next;
        n_r->next = kmem[cpu].freelist;
        kmem[cpu].freelist = n_r;
        n_r = kmem[next_cpu].freelist;
        --steal_left;
      }
      release(&kmem[next_cpu].lock);
      if(steal_left == 0) break;
    }
    acquire(&kmem[cpu].lock);
    release(&kmem[cpu].stealing_lock);
  }

  r = kmem[cpu].freelist;
  if(r)
    kmem[cpu].freelist = r->next;
  release(&kmem[cpu].lock);

  pop_off();

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
