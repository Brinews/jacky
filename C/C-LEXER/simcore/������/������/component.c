
/* 
 * Designer:   name, student id, email address 
*/

#include "spimcore.h"

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) //ALU() indicate how the ALU finish it's operation
{
	if(ALUControl==0x0)
		*ALUresult=A+B;
	if(ALUControl==1)
		*ALUresult=A-B;
	if(ALUControl==2)
	{
		if(A<B)
			*ALUresult=1;
		else
			*ALUresult=0;
	}
	if(ALUControl==4)
		*ALUresult=A&B;
	if(ALUControl==5)
		*ALUresult=A|B;
	if(ALUControl==6)
		*ALUresult=(B)<<16;
	if(ALUControl==7)
		*ALUresult=~A;
	if(*ALUresult==0)
		*Zero=1;
	else
		*Zero=0;
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	*instruction=Mem[PC>>2];
	if((PC>>2)>MEMSIZE)
		return 1;
	return 0;
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	int transfer;
	*op = instruction >> 26; 
	transfer=instruction<<6;
	*r1=instruction>>27;
	transfer=instruction<<11;
	*r2=transfer>>27;
	transfer=instruction<<16;
	*r3=transfer>>27;
	transfer=instruction<<26;
	*funct=transfer>>26;
	transfer=instruction<<16;
	*offset=transfer>>16;
	transfer=instruction<<6;
	*jsec=transfer>>6;
}



/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	if(op==0x0){//R-format
		controls->RegWrite = 1;
		controls->RegDst = 1;
		controls->ALUOp = 7;
	}
	else if(op==35)//Load instruction
	{
		controls->ALUSrc=1;
		controls->MemtoReg=1;
		controls->RegWrite=1;
		controls->MemRead=1;
		controls->ALUOp=0;
	}
	else if(op==43)//Store instruction
	{
		controls->ALUSrc=1;
		controls->RegDst=2;
		controls->MemtoReg=2;
		controls->MemWrite=1;
		controls->ALUOp=0;
	}
	else if(op==4)//Branch instruction
	{
		controls->RegDst=2;
		controls->MemtoReg=2;
		controls->Branch=1;
		controls->ALUOp=1;
	}
	else if(op==2)//Jump instruction
	{
		controls->Jump=1;
		controls->ALUOp=0;//when execute Jump instruction, the value of ALUOp is don't care
	}

	else return 1;	//invalid instruction
	return 0;
}

/* Read Register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1=Reg[r1];
	*data2=Reg[r2];
}


/* Sign Extend */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	int a;
	a=offset>>15;
	if(a==1)
	{
		a=~a;
		*extended_value=(a<<15)+offset;
	}
	if(a==0)
		*extended_value=(a<<15)+offset;
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)//the function ALU_operation() indicate what will the ALU do, the detail of the working process is in the function ALU()
{
	if(ALUSrc==1)//Once ALUSrc is activated, one of the input of the ALU is the offset field of the instruction
		data2=extended_value;
	switch(ALUOp){
		// R-type
		case 7://When the instruction is R-type, the operation of ALU is depend on the funct field in the instruction
			// funct = 0x20 = 32
			if(funct==0x20)
				ALU(data1,data2,0x0,ALUresult,Zero);//add
			if(funct==34)
				ALU(data1,data2,1,ALUresult,Zero);//subtraction
			if(funct==36)
				ALU(data1,data2,4,ALUresult,Zero);//and operation
			if(funct==37)
				ALU(data1,data2,5,ALUresult,Zero);//or operation
			if(funct==39)
				ALU(data1,data2,7,ALUresult,Zero);//not operation
			else return 1;	//invalid funct
			break;
		case 0://When the ALUOp equal to 0, ALU will do addition operation
			ALU(data1,data2,0,ALUresult,Zero);
			break;
		case 1://When the ALUOp equal to 1, ALU will do subtract operation
			ALU(data1,data2,1,ALUresult,Zero);
		default:
			return 1;		//invalid ALUop
	}
	return 0;
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if((ALUresult>>2)>MEMSIZE)
		return 1;
	if(MemRead==1&&MemWrite!=1){
		*memdata = Mem[ALUresult>>2];
	}
	else if(MemWrite==1&&MemRead!=1)
	{
		Mem[ALUresult>>2]=data2;
	}
	else
		return 1;
	return 0;
}

 
/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	//Reg[r2] = memdata;
	if(RegWrite==1)
	{
		if(MemtoReg==0&&RegDst==0)
			 Reg[r3]=ALUresult;
		else if(MemtoReg==1&&RegDst==1)
			 Reg[r2]=memdata;
	}
}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	*PC+=4;
	if(Jump==1)
		*PC=(jsec<<2)*4;
	if(Branch==1&&Zero==1)
		*PC=(*PC)+(extended_value<<2);


}
