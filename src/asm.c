#include "asm.h"
65;7400;1c#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <fnmatch.h>

typedef struct label
{
	char* id;
	unsigned short pc;
} label;

size_t num_defs = 0;
label* defs = NULL;

void create_label(const char* id)
{
	defs = realloc(defs, (++num_defs) * sizeof(label));
	defs[num_defs - 1].id = malloc(strlen(id) - 2);
	strcpy(defs[num_defs - 1].id, id);
	defs[num_defs - 1].id[strlen(id) - 3] = 0;
}

static const char* f0_opcodes[] =
{
	"MOV", "MOVI", "ADD", "SUB", "MUL", "DIV", "MOD", "AND",
	"OR", "XOR", "EQ", "GRT", "LST", "GEQ", "LEQ", "NEQ", NULL
};

static const char* f1_opcodes[] =
{
	"INC", "POP", NULL
};

static const char* f20_opcodes[] =
{
	"JMP", "JMT", "CALL", "DCALL", NULL
};

static const char* f21_opcodes[] =
{
	"HALT", "^ADD", "^SUB", "^MUL", "^DIV", "^MOD", "^AND", "^OR",
	"^XOR", "^EQ", "^GRT", "^LST", "^GEQ", "^LEQ", "^NEQ", "^LDB",
	"^LDW", "^INC", "^DEC", "^JMP", "^JMT", "^CALL", "^DCALL", "^NOT",
	"IN", "OUT", "CIN", "COUT", NULL
};

static const char* f3_opcodes[] =
{
	"POP", NULL
};

unsigned char get_index(const char* opcode, const char** set)
{
	for (int i = 0; set[i]; i++)
		if (strcmp(set[i], opcode) == 0) return i;
	return 0xff;
}

void parse_f0(const char* opcode)
{
	unsigned char o = get_index(opcode, f0_opcodes);
}

void parse_f1(const char* opcode)
{
	unsigned char o = get_index(opcode, f1_opcodes);

}

void parse_f20(const char* opcode)
{
	unsigned char o = get_index(opcode, f20_opcodes);

}

void parse_f21(const char* opcode)
{
	unsigned char o = get_index(opcode, f21_opcodes);

}

void parse_f3(const char* opcode)
{
	unsigned char o = get_index(opcode, f3_opcodes);

}

void asm_build(const char* path)
{
	DIR* dir = opendir(path);
	struct dirent* file;
	while ((file = readdir(dir)) != NULL)
	{
		if (file->d_type != DT_REG) continue;
		if (fnmatch("*.asm", file->d_name, 0) != 0) continue;
		char* source = malloc(strlen(file->d_name) + strlen(path) + 1);
		strcpy(source, path);
		strcat(source, file->d_name);
		FILE* in = fopen(source, "r");
		free(source);
		char* line = NULL;
		size_t size = 0;
		ssize_t len;
		while ((len = getline(&line, &size, in)) >= 0)
		{
			if (len == 0) continue;
			if (line[len - 2] == ':') { create_label(line); continue; }

			if (line[0] != '\t')
			{
			}
		
			const char* opcode = strtok(line, "\t ,->\n");
			printf("%s\n", opcode);
			if (get_index(opcode, f0_opcodes) != 0xff) parse_f0(opcode);
			else if (get_index(opcode, f1_opcodes) != 0xff) parse_f1(opcode);
			else if (get_index(opcode, f20_opcodes) != 0xff) parse_f20(opcode);
			else if (get_index(opcode, f21_opcodes) != 0xff) parse_f21(opcode);
			else if (get_index(opcode, f3_opcodes) != 0xff) parse_f3(opcode);
		}
		fclose(in);
	}
	closedir(dir);
	for (size_t i = 0; i != num_defs; i++) free(defs[i].id);
	free(defs);
}
