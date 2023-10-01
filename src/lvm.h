#ifndef VM_H
#define VM_H

#define MEM_SIZE (1ull << 32)
#define DISK_SIZE (1ull << 32)

extern int running;

extern unsigned int pc;
extern int reg[4];
extern int bit;
extern unsigned char* mem;
extern unsigned char disk[DISK_SIZE];

void lvm_init(const char*);
void lvm_exec();

#endif
