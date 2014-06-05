#include <iostream>
#include <iomanip>
#include "thumbsim.hpp"
#define BL_TYPE 30
#include <cstring>
using namespace std;


/* Decode to find what type it is */
Thumb_Types decode (const ALL_Types);

/* Decodes for each type */
ALU_Ops decode (const ALU_Type);
DP_Ops decode (const DP_Type);
SP_Ops decode (const SP_Type);
LD_ST_Ops decode (const LD_ST_Type);
MISC_Ops decode (const MISC_Type);
BL_Ops decode (const BL_Type);
int decode (const COND_Type);
int decode (const UNCOND_Type);
int decode (const LDM_Type);
int decode (const STM_Type);
int decode (const LDRL_Type);
int decode (const ADD_SP_Type);


bool printRegisterList(unsigned int reg_list, bool multiple) {
       if (reg_list & 1) {
         cout << "r0";
         multiple = TRUE;
       }
       if (reg_list & 2) {
         if (multiple)
           cout << ", ";
         cout << "r1";
         multiple = TRUE;
       }
       if (reg_list & 4) {
         if (multiple)
           cout << ", ";
         cout << "r2";
         multiple = TRUE;
       }
       if (reg_list & 8) {
         if (multiple)
           cout << ", ";
         cout << "r3";
         multiple = TRUE;
       }
       if (reg_list & 16) {
         if (multiple)
           cout << ", ";
         cout << "r4";
         multiple = TRUE;
       }
       if (reg_list & 32) {
         if (multiple)
           cout << ", ";
         cout << "r5";
         multiple = TRUE;
       }
       if (reg_list & 64) {
         if (multiple)
           cout << ", ";
         cout << "r6";
         multiple = TRUE;
       }
       if (reg_list & 128) {
         if (multiple)
           cout << ", ";
         cout << "r7";
         multiple = TRUE;
       }
       
       return multiple;
}


Thumb_Types decode (const ALL_Types data) {
   if (data.type.alu.instr.class_type.type_check == ALU_TYPE) {
      return ALU;
   }
   else if (data.type.dp.instr.class_type.type_check == DP_TYPE) {
      return DP;
   }//cmpr is a special type
   else if (data.type.sp.instr.class_type.type_check == SP_TYPE) {
      return SPECIAL;
   }
   else if (data.type.uncond.instr.class_type.type_check == UNCOND_TYPE) {
      return UNCOND;
   }
   else if (data.type.misc.instr.class_type.type_check == MISC_TYPE) {
      return MISC;
   }
   else if (data.type.cond.instr.class_type.type_check == COND_TYPE) {
      return COND;
   }
   else if (data.type.ldm.instr.class_type.type_check == LDM_TYPE) {
      return LDM;
   }
   else if (data.type.stm.instr.class_type.type_check == STM_TYPE) {
      return STM;
   }
   else if (data.type.ldrl.instr.class_type.type_check == LDRL_TYPE) {
      return LDRL;
   }
   else if (data.type.addsp.instr.class_type.type_check == ADD_SP_TYPE) {
      return ADD_SP;
   }
   else if (data.type.bl.instr.class_type.type_check == BL_TYPE) {
    return BL;
   }
   else {
      if (data.type.ld_st.instr.class_type.opA == LD_ST_REG_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMM_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMB_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMH_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMSP_OPA) {
      }
      else {
         cout << "The type is: " <<  data.type.ld_st.instr.class_type.opA << endl;
         cout << "NO TYPE FOUND" << endl;
         return ERROR_TYPE;
      }
      return LD_ST;
   }
}

ALU_Ops decode (const ALU_Type data) {
   if (data.instr.lsli.op == ALU_LSLI_OP) {
      if (opts.instrs) {     
         cout << "lsls r" << data.instr.lsli.rd << ", r" << data.instr.lsli.rm << ", #" << setbase(10) << data.instr.lsli.imm << endl;
      }
      return ALU_LSLI;
   }
   else if (data.instr.lsri.op == ALU_LSRI_OP) {
      if (opts.instrs) {
         cout << "lsrs r" << data.instr.lsri.rd << ", r" << data.instr.lsri.rm << ", #" << setbase(10) << data.instr.lsri.imm << endl;
      }
      return ALU_LSRI;
   }
   else if (data.instr.asri.op == ALU_ASRI_OP) {
      if (opts.instrs) {
         cout << "asrs r" << data.instr.asri.rd << ", r" << data.instr.asri.rd << ", #" << setbase(10) << data.instr.asri.imm << endl;
      }
      return ALU_ASRI;
   }
   else if (data.instr.addr.op == ALU_ADDR_OP) {
      if (opts.instrs) { 
         cout << "adds r" << data.instr.addr.rd  << ", r" << data.instr.addr.rn << ", r" << data.instr.addr.rm << endl;
      }
      return ALU_ADDR;
   }
   else if (data.instr.subr.op == ALU_SUBR_OP) {
      if (opts.instrs) {
         cout << "subs r\n" << data.instr.subr.rd  << ", r" << data.instr.subr.rn << ", r" << data.instr.subr.rm << endl;
      }
      return ALU_SUBR;
   }
   else if (data.instr.add3i.op == ALU_ADD3I_OP) {
      if (opts.instrs) { 
         cout << "adds r" << data.instr.add3i.rd << ", r" << data.instr.add3i.rn << ", #" << setbase(10) << data.instr.add3i.imm << endl;
      }
      return ALU_ADD3I;
   }
   else if (data.instr.sub3i.op == ALU_SUB3I_OP) {
      if (opts.instrs) {
         cout << "subs r" << data.instr.sub3i.rd << ", r" << data.instr.sub3i.rn << ", #" << setbase(10) << data.instr.sub3i.imm << endl;
      }
      return ALU_SUB3I;
   }
   else if (data.instr.add8i.op == ALU_ADD8I_OP) {
      if (opts.instrs) { 
         cout << "adds r" << data.instr.add8i.rdn << ", #" << setbase(10) << data.instr.add8i.imm << endl;
      }
      return ALU_ADD8I;
   }
   else if (data.instr.sub8i.op == ALU_SUB8I_OP) {
      if (opts.instrs) {
         cout << "subs r" << data.instr.sub8i.rdn << ", #" << setbase(10) << data.instr.sub8i.imm << endl;
      }
      return ALU_SUB8I;
   }
   else if (data.instr.cmp.op == ALU_CMP_OP) { 
      if (opts.instrs) { 
         cout << "cmp r" << data.instr.cmp.rdn << ", #" << setbase(10) << data.instr.cmp.imm << endl;
      }
      return ALU_CMP;
   }
   else if (data.instr.mov.op == ALU_MOV_OP) { 
      if (opts.instrs) { 
         cout << "movs r" << data.instr.mov.rdn << ", #" << setbase(10) << (data.instr.mov.imm) << endl;
      }
      return ALU_MOV;
   }
   else {
      cout << "MISSED A TYPE\n";
   }
}

//page 86
DP_Ops decode (const DP_Type data) {
   if(data.instr.dp.op == 0x0){
      if (opts.instrs) { 
         cout << "ands r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_AND;
   }
   else if(data.instr.dp.op == 0x01) {
      if (opts.instrs) { 
         cout << "eors r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_EOR;
   }
   else if(data.instr.dp.op == 0x02) {
      if (opts.instrs) { 
         cout << "lsls r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_LSL;
   }
   else if(data.instr.dp.op == 0x03) {
      if (opts.instrs) { 
         cout << "lsrs r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_LSR;
   }
   else if(data.instr.dp.op == 0x04) {
      if (opts.instrs) { 
         cout << "arsr r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_ASR;
   }
   else if(data.instr.dp.op == 0x05) {
      if (opts.instrs) { 
         cout << "adcs r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_ADC;
   }
   else if(data.instr.dp.op == 0x06) {
      if (opts.instrs) { 
         cout << "sbcs r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_SBC;
   }
   else if(data.instr.dp.op == 0x07) {
      if (opts.instrs) { 
         cout << "rsbs r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_RSB;
   }
   else if(data.instr.dp.op == 0x08) {
      if (opts.instrs) { 
         cout << "tsts r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_TST;
   }
   else if(data.instr.dp.op == 0x09) {
      if (opts.instrs) { 
         cout << "rsbs r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_RSB;
   }
   else if(data.instr.dp.op == 0x0a) {
      if (opts.instrs) { 
         cout << "cmp r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_CMP;
   }
   else if(data.instr.dp.op == 0x0b) {
      if (opts.instrs) { 
         cout << "cmn r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_CMN;
   }
   else if(data.instr.dp.op == 0x0c) {
      if (opts.instrs) { 
         cout << "orrs r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_ORR;
   }
   else if(data.instr.dp.op == 0x0d) {
      if (opts.instrs) { 
         cout << "muls r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_MUL;
   }
   else if(data.instr.dp.op == 0x0e) {
      if (opts.instrs) { 
         cout << "bics r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_BIC;
   }    
   else if(data.instr.dp.op == 0x0f) {
      if (opts.instrs) { 
         cout << "mvns r" << data.instr.dp.rdn << ", r" << data.instr.dp.rm  << endl;
      }
      return DP_MVN;
   }
   else {
      cout << "MISSED A TYPE\n";
   }
}

SP_Ops decode (const SP_Type data) {
   if (data.instr.mov.op == 2) {
      if (opts.instrs) { 
         cout << "mov";
         if (data.instr.mov.d == 1) {
           if (data.instr.mov.rd + 8 == 13) {
             cout << " sp, r" << data.instr.mov.rm << endl;
           }
           else {
              cout << " r" << data.instr.mov.rd + 8 << ", r" << data.instr.mov.rm << endl;
           }
         }
         else {
            cout << " r" << data.instr.mov.rd << ", r" << data.instr.mov.rm << endl;
         }
      }
      return SP_MOV;
   }//using cmp reg on page 129 encoding 2
   else if(data.instr.cmp.op == 1) {
      unsigned int dBit = 0;
      if(data.instr.cmp.d == 1) {
         dBit = 8;
      }

      cout << "cmp r" << data.instr.cmp.rd + dBit << ", r" << data.instr.cmp.rm <<endl;
      return SP_CMPR;
   }
   else if(data.instr.add.op == 0) {      
      if (opts.instrs) {
        if (data.instr.add.d == 1) {
          if (data.instr.mov.rd + 8 == 13) { 
            cout << "add sp, sp, r" << data.instr.add.rm << endl;
          }
          else {
            cout << "add r" << data.instr.add.rd + 8 << ", r" << data.instr.add.rd + 8
              << ", r" << data.instr.add.rm << endl;
          }
        }
        else {
//             cout << "SP_ADD case 3" << endl;
         cout << "add r" << data.instr.add.rd << ", r" << data.instr.add.rd
            << ", r" << data.instr.add.rm << endl;
        }
      }return SP_ADD;
   }
   else {
      if (opts.instrs) { 
         cout << "nop" << endl;
      }
   }

}

LD_ST_Ops decode (const LD_ST_Type data) {
   if (data.instr.class_type.opA == LD_ST_REG_OPA) {
      if (data.instr.class_type.opB == LD_ST_OPB_STR) {
         if (opts.instrs) { 
            cout << "str r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ", r" << (data.instr.ld_st_reg.rm) << "]" << endl;
         }
         return STRR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_LDR) {
         if (opts.instrs) { 
            cout << "ldr r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ", r" << (data.instr.ld_st_reg.rm) << "]" << endl;
         }
         return LDRR;
      }
      //Might not ever use this?
      else if (data.instr.class_type.opB == LD_ST_OPB_LDRB){
         if (opts.instrs) {
         cout << "ldrb r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ", r" << data.instr.ld_st_reg.rm << "]" << endl;
         }
         return LDRBR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_STRB){
         if (opts.instrs) {
            cout << "strb r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ", r" << data.instr.ld_st_reg.rm << "]" << endl;
         }
         return STRBR;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMM_OPA) {
      if (data.instr.class_type.opB == LD_ST_OPB_STR) {
         if (opts.instrs) { 
            cout << "str r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return STRI;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_LDR) {
         if (opts.instrs) { 
            cout << "ldr r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return LDRI;
      }
      //Might not ever use this?
      else if (data.instr.class_type.opB == LD_ST_OPB_LDRB){
         cout<< "***************************\n";
         if (opts.instrs) {        
            cout << "ldrb r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ",  #" << setbase(10) << (data.instr.ld_st_imm.imm) << "]" << endl;
         }
         return LDRBI;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_STRB){
         if (opts.instrs) {
            cout << "strb r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ",  #" << setbase(10) << (data.instr.ld_st_imm.imm) << "]" << endl;
         }
         return STRBI;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMMB_OPA) {
      if (data.instr.class_type.opB == LD_ST_OPB_LDR){           
         if (opts.instrs) {        
            cout << "ldrb r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ",  #" << setbase(10) << (data.instr.ld_st_imm.imm) << "]" << endl;
         }
         return LDRBI;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_STR){
         if (opts.instrs) {
            cout << "strb r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ",  #" << setbase(10) << (data.instr.ld_st_imm.imm) << "]" << endl;
         }
         return STRBI;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMMH_OPA) {
   cout << "==>add 24\n";
   }
   else if (data.instr.class_type.opA == LD_ST_IMMSP_OPA) {
   cout << "==>add 25\n";
   }
   else {
      cout << "MISSED A TYPE\n";
   }
}

MISC_Ops decode (const MISC_Type data) {
   if (data.instr.push.op == MISC_PUSH_OP) {
      if (opts.instrs) { 
       bool multiple = FALSE;
       cout << "push ";
       cout << "{";
       
       multiple = printRegisterList(data.instr.push.reg_list, multiple);
       
       if (data.instr.push.m) {
         if (multiple)
           cout << ", ";
         cout << "lr";
       }
       cout << "}" << endl;
      }
      return MISC_PUSH;
   }
   else if (data.instr.pop.op == MISC_POP_OP) {
     if (opts.instrs) { 
       bool multiple = FALSE;
       cout << "pop ";
       cout << "{";
       
       multiple = printRegisterList(data.instr.pop.reg_list, multiple);
       
       if (data.instr.pop.m) {
         if (multiple)
           cout << ", ";
         cout << "pc";
       }
       cout << "}" << endl;
     }
      return MISC_POP;
   }
   else if (data.instr.sub.op == MISC_SUB_OP) {
      if (opts.instrs) { 
         cout << "sub sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.instr.sub.imm)<< 2) << endl;
      } 
      return MISC_SUB;
   }
   else if (data.instr.add.op == MISC_ADD_OP) {
      if (opts.instrs) { 
         cout << "add sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.instr.add.imm)<< 2) << endl;
      } 
      return MISC_ADD;
   }
   else if (data.instr.class_type.type_check == ADD_SP8I_TYPE) {
      if (opts.instrs) { 
         cout << "add sp, #" << setbase(10) << (data.instr.add.imm<<2)<< endl;
      }
      return MISC_ADD;
   }
   else {
      cout << "MISSED A TYPE\n";
   }

}

int decode (const COND_Type data) {
   if (opts.instrs) { 
      cout << "b";
      printCond(data.instr.b.cond);
      cout << " 0x" << hex << data.instr.b.imm << endl;
   }
   
   return COND_TYPE;
}

int decode (const UNCOND_Type data) {
   if (opts.instrs) { 
      cout << "b 0x" << hex << data.instr.b.imm << endl;
   }
 
   return UNCOND_TYPE;
}

int decode (const LDM_Type data) {
   unsigned int reg = data.instr.ldm.rn ;
   if (opts.instrs) {
      cout << "ldmia r" << reg << "!";
      if((data.instr.ldm.reg_list & (1 << reg)) == 1) {
         cout << "!, {";
      }
      else { 
         cout <<  ", {";
      }
      
      printRegisterList(data.instr.ldm.reg_list, FALSE);
      
      cout << "}"<< endl;
   }
   return LDM_TYPE;
}

int decode (const STM_Type data) {
   unsigned int reg = data.instr.stm.rn ;

   if (opts.instrs)    {
      cout << "stm r" << reg << "!, {";
      
      printRegisterList(data.instr.stm.reg_list, FALSE);
      
      cout << "}" << endl;
   }
   return STM_TYPE;
}

int decode (const LDRL_Type data) {
   if (opts.instrs)
      cout << "ldr r" << data.instr.ldrl.rt << ", [pc, #" << setbase(10) << data.instr.ldrl.imm* 4 << "]\n";
   
   return LDRL_TYPE;
}

int decode (const ADD_SP_Type data) {
   if (opts.instrs) { 
      cout << "add r" << data.instr.add.rd << ", sp, #" << setbase(10) << data.instr.add.imm * 4 << endl;
   }
   return ADD_SP_TYPE;
}

BL_Ops decode (const BL_Type data) {
  if (opts.instrs) { 
    cout << "bl 0x" << hex << data.instr.bl_upper.imm10 << endl;
  }
  return BL_UPPER;
}