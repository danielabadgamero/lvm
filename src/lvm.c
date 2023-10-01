#include "lvm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int running = 0;
unsigned int pc = 0;
int reg[4];
unsigned char* mem;
unsigned char disk[DISK_SIZE];

static size_t fsize(FILE* file)
{
	size_t pos = ftell(file);
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, pos, SEEK_SET);
	return size;
}

void lvm_init(const char* name)
{
	mem = malloc(MEM_SIZE);

	char* dir_name = malloc(5 + strlen(name));

	strcpy(dir_name, "prog/");
	strcat(dir_name, name);
	const char* bin_path = malloc(strlen(dir_name) + 4);
	bin_path = strcat(dir_name, "/bin");
	FILE* bin = fopen(bin_path, "rb");

	size_t size = fsize(bin);
	if (size > (1 << 16))
	{
		printf("Programme is too big");
		return;
	}

	fread(mem, 1, size, bin);

	fclose(bin);

	running = 1;
}

void lvm_exec()
{
	if (mem[pc] == 0) running = 0;
}

void lvm_quit()
{
	free(mem);
}
