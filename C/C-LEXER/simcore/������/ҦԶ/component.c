
/* 
 * Designer:   name, student id, email address 
*/

#include "spimcore.h"

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	if(ALUControl==0x0)*ALUresult=A+B;//add
	if(ALUControl==0x1)*ALUresult=A-B;//subtraction
	if(ALUControl==0x2){
		if(A<B)*ALUresult=1;
		else*ALUresult=0;
	}
	if(ALUControl==0x3){
		if(A<B)*ALUresult=1;
		else*ALUresult=0;
	}
	if(ALUControl==0x4)*ALUresult=A & B;
	if(ALUControl==0x5)*ALUresult=A || B;
	if(ALUControl==0x6)*ALUresult=B << 0x10;
	if(ALUControl==0x7)*ALUresult!=A;
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	if(*instruction%4){//The addresses of all instructions are multple of 4
	*instruction=Mem[PC>>2];
	return 0;
	}
	else return 1;//invaild instruction
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op = instruction >> 26; 
	*r1 = instruction >> 21 & 0x1F;
	*r2 = instruction >> 16 & 0x19;
	*r3 = instruction >> 11 & 0x14;
	*funct = instruction >> 0 & 0xF;
	*offset = instruction >> 0 & 0x5;
	*offset = instruction << 15;
	*jsec = instruction >> 0 ;
	*jsec = instruction << 25;
}



/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	if(op==0x0){//R-format
		controls->RegDst = 1;
		controls->ALUSrc = 0;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 0;
		controls->MemWrite = 0;
		controls->ALUOp = 7;
	}
	else return 1;	//invalid instruction
	return 0;

	if(op==0x8){//addi
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->ALUOp = 0;
	}
	else return 1;	//invalid instruction
	return 0;

	if(op==0xA){//slt
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 1;
		controls->RegWrite = 1;
		controls->MemRead = 1;
		controls->MemWrite = 0;
		controls->ALUOp = 2;
	}
	else return 1;	//invalid instruction
	return 0;

	if(op==0xB){//slti
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->ALUOp = 3;
	}
	else return 1;	//invalid instruction
	return 0;

	if(op==0xC){//andi
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->ALUOp = 0;
	}
	else return 1;	//invalid instruction
	return 0;
	if(op==0xD){//ori
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->ALUOp = 0;
	}
	if(op==0xF){//lui
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->ALUOp = 0;
	}
	else return 1;	//invalid instruction
	return 0;
}

/* Read Register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	*extended_value = offset;
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	switch(ALUOp){
		// R-type
		case 7:
			// funct = 0x20 = 32
			if(funct==0x20)ALU(data1,data2,0x0,ALUresult,Zero);	//add
			if(funct==0x22)ALU(data1,data2,0x1,ALUresult,Zero); //subtract
			if(funct==0x2A)ALU(data1,data2,0x2,ALUresult,Zero);//slt
			if(funct==0x2B)ALU(data1,data2,0x3,ALUresult,Zero);//slt unsigned
			if(funct==0x24)ALU(data1,data2,0x4,ALUresult,Zero);//and
			if(funct==0x25)ALU(data1,data2,0x5,ALUresult,Zero);//or
			else return 1;	//invalid funct
			break;
		case 0:
			ALU(data1,data2,0x0,ALUresult,Zero);//add
		case 1:
			ALU(data1,data2,0x1,ALUresult,Zero);//subtract
		case 2:
			ALU(data1,data2,0x2,ALUresult,Zero);//slt
		case 3:
			ALU(data1,data2,0x3,ALUresult,Zero);//slt unsigned
		case 4:
			ALU(data1,data2,0x4,ALUresult,Zero);//and
		case 5:
			ALU(data1,data2,0x5,ALUresult,Zero);//or
		case 6:
			ALU(data1,data2,0x6,ALUresult,Zero);//shift
		default:
			return 1;		//invalid ALUop
	}
	return 0;
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if(MemRead==1){
		*memdata = Mem[ALUresult>>2];
	}
	if(MemWrite==1){
		Mem[ALUresult>>2] = *memdata;
	}
	return 0;
}

 
/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	Reg[r2] = memdata;
	Reg[r3] = ALUresult;
	Reg[r3] = RegWrite;
	Reg[r3] = RegDst;
	Reg[r2] = MemtoReg;
}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	if(Branch = 1){
	*PC+=4;		
	}
}
