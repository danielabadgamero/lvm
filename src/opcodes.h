#ifndef OPCODES_H
#define OPCODES_H

extern void (*f0_opcodes[16])(int, int, int, int, int);
extern void (*f1_opcodes[2])(int, int, int);
extern void (*f20_opcodes[4])(int);
extern void (*f21_opcodes[32])();
extern void (*f3_opcodes[1])(int, int, int);

#endif
