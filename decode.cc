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
      Data16 instr;
      memcpy(&instr, &data, sizeof(Data16));
   if (data.type.alu.instr.class_type.type_check == ALU_TYPE) {
      return ALU;
   }
   else if (data.type.dp.instr.class_type.type_check == DP_TYPE) {
      return DP;
   }
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
      //THIS IS A HACK BECAUSE BL DOES NOT SEEM TO BE HANDLED...
     /*else if((instr.data_ushort() & (0xF8 << 8)) >> 11 == 0x1E) {
         return BL;
      }
      */else {
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
         cout << "lsrs r" << data.instr.lsli.rd << ", r" << data.instr.lsli.rd << ", #" << data.instr.lsli.imm << endl;
      }
      return ALU_LSLI;
   }
   else if (data.instr.lsri.op == ALU_LSRI_OP) {
      if (opts.instrs) {
         cout << "lsrs r" << data.instr.lsri.rd << ", r" << data.instr.lsri.rd << ", #" << data.instr.lsri.imm << endl;
      }
      return ALU_LSRI;
   }
   else if (data.instr.asri.op == ALU_ASRI_OP) {
      if (opts.instrs) {
         cout << "asrs r" << data.instr.asri.rd << ", r" << data.instr.asri.rd << ", #" << data.instr.asri.imm << endl;
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
         cout << "adds r" << data.instr.add3i.rd << ", r" << data.instr.add3i.rn << ", #" << data.instr.add3i.imm << endl;
      }
      return ALU_ADD3I;
   }
   else if (data.instr.sub3i.op == ALU_SUB3I_OP) {
      if (opts.instrs) {
         cout << "subs r" << data.instr.sub3i.rd << ", r" << data.instr.sub3i.rn << ", #" << data.instr.sub3i.imm << endl;
      }
      return ALU_SUB3I;
   }
   else if (data.instr.add8i.op == ALU_ADD8I_OP) {
      if (opts.instrs) { 
         cout << "adds r" << data.instr.add8i.rdn << ", #" << data.instr.add8i.imm << endl;
      }
      return ALU_ADD8I;
   }
   else if (data.instr.sub8i.op == ALU_SUB8I_OP) {
      if (opts.instrs) {
         cout << "subs r" << data.instr.sub8i.rdn << ", #" << data.instr.sub8i.imm << endl;
      }
      return ALU_SUB8I;
   }
   else if (data.instr.cmp.op == ALU_CMP_OP) { 
      if (opts.instrs) { 
         cout << "cmp r" << data.instr.cmp.rdn << ", #" << data.instr.cmp.imm << endl;
      }
      return ALU_CMP;
   }
   else if (data.instr.mov.op == ALU_MOV_OP) { 
      if (opts.instrs) { 
         cout << "movs r" << data.instr.mov.rdn << ", #" << (data.instr.mov.imm) << endl;
      }
      return ALU_MOV;
   }

}
DP_Ops decode (const DP_Type data) {
   cout << "DP_TYPE" << endl;
}

SP_Ops decode (const SP_Type data) {
   if (data.instr.mov.op == 2) {
      if (opts.instrs) { 
         cout << "mov";
         if (data.instr.mov.d) {
            cout << " sp, r" << data.instr.mov.rm << endl;
         }
         else {
            cout << " r" << data.instr.mov.rd << ", r" << data.instr.mov.rm << endl;
         }
      }
      return SP_MOV;
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
         //NOT SURE IF RIGHT RETURN CODE
         return STRR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_LDR) {
         if (opts.instrs) { 
            cout << "ldr r" << data.instr.ld_st_reg.rt << ", [r" << data.instr.ld_st_reg.rn << ", r" << (data.instr.ld_st_reg.rm) << "]" << endl;
         }
         //NOT SURE IF RIGHT RETURN CODE
         return LDRR;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMM_OPA) {
      if (data.instr.class_type.opB == LD_ST_OPB_STR) {
         if (opts.instrs) { 
            cout << "str r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return STRR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_LDR) {
         if (opts.instrs) { 
            cout << "ldr r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return LDRR;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMMB_OPA) {
   cout << "==>add 23\n";
   }
   else if (data.instr.class_type.opA == LD_ST_IMMH_OPA) {
   cout << "==>add 24\n";
   }
   else if (data.instr.class_type.opA == LD_ST_IMMSP_OPA) {
   cout << "==>add 25\n";
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
      cout << "ldm r" << reg;
      if((data.instr.ldm.reg_list & (1 << reg)) == 1) {
         cout << "!, ";
      }
      else { 
         cout <<  ", ";
      }
      
      printRegisterList(data.instr.ldm.reg_list, FALSE);
      
      cout << endl;
   }
   return LDM_TYPE;
}

int decode (const STM_Type data) {
   unsigned int reg = data.instr.stm.rn ;
  
   if (opts.instrs)    {
      cout << "str r" << reg << "! ,";
      
      printRegisterList(data.instr.stm.reg_list, FALSE);
      
      cout << endl;
   }
   return STM_TYPE;
}

int decode (const LDRL_Type data) {
   if (opts.instrs)
      cout << "ldrl r" << data.instr.ldrl.rt << ", #" << setbase(10) << data.instr.ldrl.rt * 4 << endl;
   
   return LDRL_TYPE;
}

int decode (const ADD_SP_Type data) {
   if (opts.instrs) { 
      cout << "add r" << data.instr.add.rd << ", sp, #" << setbase(10) << data.instr.add.imm << endl;
   }
   return ADD_SP_TYPE;
}
