#include <string.h>

#include <kernel/isrs.h>

struct idt_entry
{
  unsigned short base_lo;
  unsigned short sel;
  unsigned char always0;
  unsigned char flags;
  unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
  idt[num].base_lo = base & 0xFFFF;
  idt[num].base_hi = (base >> 16) & 0xFFFF;
  idt[num].sel     = sel;
  idt[num].always0 = 0;
  idt[num].flags   = flags; /* | 0x60 for laaaaaaater set the privilege to 3*/
}

void idt_initialize()
{
  idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
  idtp.base = (unsigned int) &idt;

  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  isrs_install();
  
  idt_load();
}
