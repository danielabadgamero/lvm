#include "Lightning.h"

struct Bytes
{
	int start{};
	int size{};
};

void Lightning::Op::operator=(char byte)
{
	opcode = (byte & 0xf0) >> 4;
	dest = (byte & 0x08) >> 3;
	ddMode = (byte & 0x04) >> 2;
	src = (byte & 0x02) >> 1;
	sdMode = byte & 0x01;
}

static Bytes getBytes(unsigned char reg)
{
	Bytes bytes{ 0, 8 };

	int selBytes{ (reg & 0x30) >> 4 };
	if (selBytes == 0b11) return bytes;
	bytes.size = 4;
	if (selBytes == 0b01)
		bytes.start += 4;

	selBytes = (reg & 0x0c) >> 2;
	if (selBytes == 0b11) return bytes;
	bytes.size = 2;
	if (selBytes == 0b01)
		bytes.start += 2;

	selBytes = (reg & 0x03);
	if (selBytes == 0b11) return bytes;
	bytes.size = 1;
	if (selBytes == 0b01)
		bytes.start += 1;

	return bytes;
}

void Lightning::reset()
{
	memset(RAM, 0, 1ll << 16);
	memset(reg, 0, 4 * sizeof(long long));
	pc = pb = 0;
	flag.reset();
	while (!stack.empty()) stack.pop();
}

void Lightning::loop()
{
	op = RAM[pc];
	pc++;
	if (op.dest == 0) (op.dReg = RAM[pc]), pc++;
	else (op.dImm = (RAM[pc] << 8) | RAM[pc + 1]), pc += 2;
	if (op.src == 0) (op.sReg = RAM[pc]), pc++;
	else (op.sImm = (RAM[pc] << 8) | RAM[pc + 1]), pc += 2;

	long long src{};
	char* dest{};
	int destSize{};

	if (op.src == 0)
	{
		Bytes bytes{ getBytes(op.sReg) };
		unsigned char* dBytes{ (unsigned char*)&src + (7ull - bytes.start) };
		for (int b{ bytes.size - 1 }; b >= 0; b--)
		if (op.sdMode == 0)
			*(dBytes - b) = (reg[(op.sReg & 0xc0) >> 6] & (0xffull << (bytes.size - b - 1) * 8)) >> (bytes.size - b - 1) * 8;
		else
			*(dBytes - b) = (RAM[reg[(op.sReg & 0xc0) >> 6]] & (0xffull << (bytes.size - b - 1) * 8)) >> (bytes.size - b - 1) * 8;
	}
	else
		if (op.sdMode == 0) src = op.sImm;
		else src = RAM[op.sImm];

	if (op.dest == 0)
	{
		Bytes bytes{ getBytes(op.dReg) };
		destSize = bytes.size;
		if (op.ddMode == 0)
			dest = (char*)&reg[(op.sReg) >> 6] + (7ull - bytes.start);
		else
			dest = &RAM[reg[(op.sReg) >> 6] + (7ull - bytes.start)];
	}
	else
		if (op.ddMode == 0) dest = &RAM[op.dImm];
		else dest = &RAM[RAM[op.dImm]];
			
	switch (op.opcode)
	{
	case 0x00:
		break;
	case 0x01:
		break;
	case 0x02:
		break;
	case 0x03:
		break;
	case 0x04:
		break;
	case 0x05:
		break;
	case 0x06:
		break;
	case 0x07:
		break;
	case 0x08:
		break;
	case 0x09:
		break;
	case 0x0a:
		break;
	case 0x0b:
		break;
	case 0x0c:
		break;
	case 0x0d:
		break;
	case 0x0e:
		break;
	case 0x0f:
		break;
	}
}