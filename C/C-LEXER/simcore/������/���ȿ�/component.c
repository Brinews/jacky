
/* 
 * Designer:   name, student id, email address 
*/

#include "spimcore.h"

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	if(ALUControl==0x0)*ALUresult=A+B;//add
	if(ALUControl==0x1)*ALUresult=A-B;//sub
	if(ALUControl==0x2)//signed compare
          if(A<B)*ALUresult=1;
          else   *ALUresult=0;
	if(ALUControl==0x3)//unsigned compare
	      if(A<B)*ALUresult=1;
          else   *ALUresult=0;
	if(ALUControl==0x4)*ALUresult=A & B;//
	if(ALUControl==0x5)*ALUresult=A | B;//
	if(ALUControl==0x6)*ALUresult=B << 16;//
	if(ALUControl==0x7)*ALUresult= ~A;//
	if (*ALUresult==0) *Zero = 1;
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

	*instruction=Mem[PC>>2];
	if(*instruction == 0) return 1;
	return 0;
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
     unsigned *ttmp;
	*op   = instruction >> 26; 
	
	*ttmp = instruction << 6;
	*r1   = *ttmp >> 29;
	
	*ttmp = instruction << 11;
	*r2   = *ttmp >> 29;
	
	*ttmp = instruction << 16;
	*r3   = *ttmp >> 29;
	
	*ttmp = instruction << 26;
	*funct   = *ttmp >> 26;
	
	*ttmp = instruction << 16;
	*offset   = *ttmp >> 16;
	
	*ttmp = instruction << 6;
	*jsec   = *ttmp >> 6;
	
}



/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	if(op==0x00){//ALU instruction
        controls->RegWrite = 1;
		controls->ALUOp = 7;
		controls->RegDst = 1;
		controls->ALUSrc = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 2;
		controls->MemtoReg = 0;
		controls->MemWrite = 0;		
	}
	else if(op==0x08){//ADDI struction
        controls->RegWrite = 1;
		controls->ALUOp = 0;
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 2;
		controls->MemtoReg = 0;
		controls->MemWrite = 0;		
	}
   	else if(op==0x0F){//lui struction
        controls->RegWrite = 1;
		controls->ALUOp = 6;
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 2;
		controls->MemtoReg = 0;
		controls->MemWrite = 0;	
	}
	else if(op==0x23){//lw struction
        controls->RegWrite = 1;
		controls->ALUOp = 0;
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 1;
		controls->MemtoReg = 1;
		controls->MemWrite = 0;	
	}
	else if(op==0x2B){//sw struction
        controls->RegWrite = 0;
		controls->ALUOp = 0;
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 2;
		controls->MemtoReg = 2;
		controls->MemWrite = 1;	
	}
	else if(op==0x04){//beq struction
        controls->RegWrite = 0;
		controls->ALUOp = 1;
		controls->RegDst = 0;
		controls->ALUSrc = 0;
		controls->Jump = 0;
		controls->Branch = 1;
		controls->MemRead = 2;
		controls->MemtoReg = 2;
		controls->MemWrite = 0;	
	}
	else if(op==0x0A){//slti struction
        controls->RegWrite = 1;
		controls->ALUOp = 2;
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 2;
		controls->MemtoReg = 0;
		controls->MemWrite = 0;	
	}
	else if(op==0x0B){//sltiu struction
        controls->RegWrite = 1;
		controls->ALUOp = 3;
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 2;
		controls->MemtoReg = 0;
		controls->MemWrite = 0;
	}
	else if(op==0x02){//jump struction
        controls->RegWrite = 0;
		controls->ALUOp = 0;
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->Jump = 1;
		controls->Branch = 0;
		controls->MemRead = 2;
		controls->MemtoReg = 2;
		controls->MemWrite = 2;	
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
 if (offset<0)
     *extended_value = offset + 0xffff0000;                
  else 
     *extended_value = offset; 
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	switch(ALUOp){
		// R-type
		case 7:
			// funct = 0x20 = 32
			if((funct==0x20)&&(ALUSrc==0))ALU(data1,data2,0x0,ALUresult,Zero);	//add
	        else if((funct==0x22)&&(ALUSrc==0))ALU(data1,data2,0x1,ALUresult,Zero);	//sub
	        else if((funct==0x24)&&(ALUSrc==0))ALU(data1,data2,0x4,ALUresult,Zero);	//and
	        else if((funct==0x25)&&(ALUSrc==0))ALU(data1,data2,0x5,ALUresult,Zero);	//or
	        else if((funct==0x2A)&&(ALUSrc==0))ALU(data1,data2,0x2,ALUresult,Zero);	//slt
	        else if((funct==0x2B)&&(ALUSrc==0))ALU(data1,data2,0x3,ALUresult,Zero);	//sltu
			else return 1;	//invalid funct
			break;
		case 0: if(ALUSrc==0) ALU(data1,extended_value,0x0,ALUresult,Zero);//addi
                 else return 1;	
        case 1: if(ALUSrc==0) ALU(data1,extended_value,0x1,ALUresult,Zero);	//sub
                 else return 1;
        case 2: if(ALUSrc==0) ALU(data1,extended_value,0x2,ALUresult,Zero);	//slti
                 else return 1;
        case 3: if(ALUSrc==0) ALU(data1,extended_value,0x3,ALUresult,Zero);	//sltiu
                 else return 1;
        case 6: if(ALUSrc==0) ALU(data1,extended_value,0x6,ALUresult,Zero);	//lui
                 else return 1;
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
		if (*memdata==0) return 1;
	}
	else if(MemWrite==1){
        Mem[ALUresult>>2] =  data2;            
                    }
  	return 0;
}

 
/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char C,unsigned *Reg)
{
	if(RegWrite==1){
        if((C==1)&&(RegDst==0)){
               Reg[r2] = memdata;
               }
        else
        {
         Reg[r3] = memdata;   
        }
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
