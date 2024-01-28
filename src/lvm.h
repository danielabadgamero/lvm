#ifndef VM_H
#define VM_H

#define RAM_SIZE (1ull << 16)
#define DISK_SIZE (1ull << 16)

extern int running;
extern unsigned short pc;
extern int registers[8];
extern int T;
extern unsigned char* RAM;
extern unsigned char disk[DISK_SIZE][512];
extern unsigned short call_stack[1000];
extern unsigned short* call_stack_ptr;
extern int data_stack[1000];
extern int* data_stack_ptr;

void lvm_init(const char*);
void lvm_exec();
void lvm_quit();

#endif
