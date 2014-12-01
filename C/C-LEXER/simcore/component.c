
/* 
 * Designer:   name, student id, email address 
*/

#include "spimcore.h"

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	if(ALUControl==0x0)*ALUresult=A+B;//add
	if(ALUControl==0x1)*ALUresult=A-B;//sub
	if(ALUControl==0x4)*ALUresult=A&B;//and
	if(ALUControl==0x5)*ALUresult=A|B;//or
	if(ALUControl==0x2)*ALUresult=(signed)A<(signed)B?1:0;//slt
	if(ALUControl==0x3)*ALUresult=A<B?1:0;//sltu
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	*instruction=Mem[PC>>2];
	return 0;
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op = instruction >> 26; 

	/* added code  */

	/* r3 code */
	if ( *op == 0x0 /* add, sub, and, or */ 
			|| *op == 0x0 /* slt, sltu */ ) {

		/* get funct */
		*funct = instruction & 0xFF;
		instruction = instruction - (*funct);

		*r1 = (instruction >> 21) & 0x1F; /* $s2 */
		*r2 = (instruction >> 16) & 0x1F; /* $s3 */
		*r3 = (instruction >> 11) & 0x1F; /* $s1 */
	}

	/* in code */
	if (*op == 0xa /* slti */
			|| *op == 0xb /* sltiu */ 
			|| *op == 0x8 /* addi */ ){
		*r3 = (instruction >> 16) & 0x1F; /* $s1 */
		*r1 = (instruction >> 21) & 0x1F; /* $s2 */

		*offset = instruction - (*op<<26) - (*r3<<16) - (*r1<<21);
	}

	/* il code */
	if (*op == 0x4  /* beq */ 
			|| *op == 0x5) /* bne */ {
		*r1 = (instruction >> 21) & 0x1F;
		*r2 = (instruction >> 16) & 0x1F;
		*offset = instruction - (*op<<26) - (*r1<<21) - (*r2<<16);
	}

	/* ib code */
	if (*op == 0x23 /* lw */
			|| *op == 0x2b) /* sw */ {
		*r2 = (instruction >> 16) & 0x1F; /* $s1 */
		*r1 = (instruction >> 21) & 0x1F; /* $s2 */
		*offset = instruction - (*op<<26) - (*r2<<16) - (*r1<<21);
	}

	/* r2 code */

	/* ii code */
	if (*op == 0xF) /* lui */ {
		*r3 = (instruction >> 16) & 0x1F;
		*offset = instruction - (*op<<26) - (*r3<<16);
	}

	/* ra code */

	/* rb code */

	/* j code */
	if (*op == 0x2) /* j */ {
		*jsec = instruction - (*op << 26);
	}
}

/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	if(op==0x0){//R-format
		controls->RegWrite = 1;
		controls->RegDst = 1;
		controls->ALUOp = 7;
	}
	else if (op == 0x8) { /* addi */
		controls->RegWrite = 1;
		controls->RegDst = 1;
		controls->ALUOp = 8;
	}
	else if (op == 0x4) {
		controls->Branch = 1;
		controls->ALUOp = 0; /* beq */
	}
	else if (op == 0x2) {
		controls->Jump = 1; /* j */
		controls->ALUOp = 0;
	}
	else if (op == 0x23) { /* lw */
		controls->RegWrite = 1;
		controls->ALUSrc = 1;
		controls->MemtoReg = 1;
		controls->ALUOp = 10;
	}
	else if (op == 0x2b) { /* sw */
		controls->ALUSrc = 1;
		controls->MemWrite = 1;
		controls->ALUOp = 11;
	}
	else if (op == 0xa) { /* slti */
		controls->RegWrite = 1;
		controls->RegDst = 1;
		controls->ALUOp = 12;
	}
	else if (op == 0xb) {
		controls->RegWrite = 1;
		controls->RegDst = 1;
		controls->ALUOp = 13; /* sltiu */
	}
	else if (op == 0xF) /* lui */ {
		controls->RegWrite = 1;
		controls->RegDst = 1;
		controls->MemtoReg = 0;
		controls->ALUOp = 14;
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
	int b = offset >> 15;
	if (b & 0x1) b = ~b;

	*extended_value = offset + (b<<15);
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	switch(ALUOp){
		// R-type
		case 7:
			// funct = 0x20 = 32
			if(funct==0x20)ALU(data1,data2,0x0,ALUresult,Zero);	//add
			else if (funct==0x22) ALU(data1, data2, 0x1,ALUresult, Zero);//sub
			else if (funct==0x24) ALU(data1, data2, 0x4, ALUresult, Zero);//and
			else if (funct==0x25)ALU(data1,data2,0x5,ALUresult,Zero);//or
			else if (funct==0x2a)ALU(data1,data2,0x2,ALUresult,Zero);//slt
			else if (funct==0x2b)ALU(data1,data2,0x3,ALUresult,Zero);//sltu
			else return 1;	//invalid funct
			break;
		case 8:
			ALU(data1, extended_value, 0x0, ALUresult, Zero);//addi
			break;
		case 9: /* beq */
			if (data1 == data2) *Zero = 1;
			else Zero = 0;
			break;
		case 10: /* lw */
			*ALUresult = data1 + extended_value;
			break;
		case 11: /* sw */
			*ALUresult = data1 + extended_value;
			break;
		case 12: /* slti */
			*ALUresult = data1 < (signed)extended_value ? 1 : 0;
			break;
		case 13: /* sltiu */
			*ALUresult = data1 < extended_value ? 1 : 0;
			break;
		case 14: /* lui */
			*ALUresult = extended_value*2^16;
			break;
		case 0: /* jump */
			break;
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
	else if (MemWrite == 1) {
		Mem[ALUresult>>2] = data2;
	}

	return 0;
}

 
/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if (MemtoReg)
		Reg[r2] = memdata;

	if (RegWrite == 1) {
		if (RegDst == 1) {
				Reg[r3] = ALUresult;
		}
	}
}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	*PC+=4;

	if (Branch&Zero) {
		*PC += 4*extended_value;
	}

	if (Jump) {
		*PC = jsec;
	}
}
