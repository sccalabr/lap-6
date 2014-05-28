#include  <iostream>
#include  <iomanip>
#include "thumbsim.hpp"
#define PC_REG 15
#define LR_REG 14
#define SP_REG 13
#define PC rf[PC_REG]
#define LR rf[LR_REG]
#define SP rf[SP_REG]
#define LOGGER 0

ASPR flags;
enum OFType { OF_ADD, OF_SUB, OF_SHIFT };

unsigned int signExtend16to32ui(short i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

void updateFlagsRegisterAndImmediateValue(ALU_Type alu) {
   int regValue = rf[alu.instr.cmp.rdn];
   int immValue = alu.instr.cmp.imm;
   int difference = regValue - immValue;

   if(difference == 0) {
      flags.Z = 1;
   }
   else {
      flags.Z = 0;
   }
   //I do not think this can happen since arm is 16 bits
   // This is a carry
   if(FALSE) {
      flags.C = 1;
   }
   else {
      flags.C = 0;
   }
   
   if(difference < 0) {
      flags.N = 1;
   }
   else {
      flags.N = 0;
   }
   //I do not think this can happen since arm is 16 bits
   //This is overflow
   if(FALSE) {
      flags.V = 1;
   }
   else {
      flags.V = 0;
   }
}


void setCarryOverflow (int num1, int num2, OFType oftype) {

  switch (oftype) {
    case OF_ADD:
      if (((unsigned long long int)num1 + (unsigned long long int)num2) ==
          ((unsigned int)num1 + (unsigned int)num2)) {
        flags.C = 0;
      }
      else {
        flags.C = 1;
      }
      if (((long long int)num1 + (long long int)num2) ==
          ((int)num1 + (int)num2)) {
        flags.V = 0;
      }
      else {
        flags.V = 1;
      }
      break;
    case OF_SUB:
      if (((unsigned long long int)num1 - (unsigned long long int)num2) ==
          ((unsigned int)num1 - (unsigned int)num2)) {
        flags.C = 0;
      }
      else {
        flags.C = 1;
      }
      if (((num1==0) && (num2==0)) || 
          (((long long int)num1 - (long long int)num2) ==
          ((int)num1 - (int)num2))) {
        flags.V = 0;
      }
      else {
        flags.V = 1;
      }
      break;
    case OF_SHIFT:
      // C flag unaffected for shifts by zero
      if (num2 != 0) {
        if (((unsigned long long int)num1 << (unsigned long long int)num2) ==
            ((unsigned int)num1 << (unsigned int)num2)) {
          flags.C = 0;
        }
        else {
          flags.C = 1;
        }
      }
      // Shift doesn't set overflow
      break;
    default:
      cerr << "Bad OverFlow Type encountered." << __LINE__ << __FILE__ << endl;
      exit(1);
  }
}

// You're given the code for evaluating BEQ, 
// and you'll need to fill in the rest of these.
// See Page 99 of the armv6 manual
static int checkCondition(unsigned short cond) {
   switch(cond) {
      case EQ:
         if (flags.Z == 1) {
            return TRUE;
         }
         break;
      case NE:
         if (flags.Z == 0) {
            return TRUE;
         }
         break;
      case CS:
         if (flags.C == 1) {
            return TRUE;
         }
         break;
      case CC:
         if (flags.C == 0) {
            return TRUE;
         }
         break;
      case MI:
         if (flags.N == 1) {
            return TRUE;
         }
         break;
      case PL:
         if (flags.N == 0) {
            return TRUE;
         }
         break;
      case VS:
         if (flags.V == 1) {
            return TRUE;
         }
         break;
      case VC:
         if (flags.V == 0) {
            return TRUE;
         }
         break;
      case HI:
         if (flags.C == 1 && flags.Z == 0) {
            return TRUE;
         }
         break;
      case LS:
         if (flags.C == 0  || flags.Z == 1) {
            return TRUE;
         }
         break;
      case GE:
         if (flags.V == flags.N) {
            return TRUE;
         }
         break;
      case LT:
         if (flags.V != flags.N) {
            return TRUE;
         }
         break;
      case GT:
         if (flags.Z == 0 && flags.V == flags.N) {
            return TRUE;
         }
         break;
      case LE:
         if (flags.Z == 1 || flags.V != flags.N) {
            return TRUE;
         }
         break;
      case AL:
         return TRUE;
         break;
   }
   return FALSE;
}

int countOneBits(unsigned int instruction) {
   instruction &= 0x00FF;
   int counter = 0;
   
   for( ; instruction; counter++) {
         instruction = instruction & (instruction - 1);
   }

   return counter;
}

void pushRegistersOntoStack(MISC_Type misc) {
   unsigned int numberOfRegisters = countOneBits(misc.instr.push.reg_list);
   unsigned int spAddress = SP - 4 * numberOfRegisters;
   unsigned int i, mask;
   
   rf.write(LR_REG, SP);

   /* pg167 I am using 8 because thumb has 
      8 registers in the list. The document
      says to use 14, so this may cause a problem 
      later.
    */
   for(i = 0, mask = 1; i < 8; i++, mask <<= 1) {
      if(misc.instr.push.reg_list & mask) {
         dmem.write(spAddress, rf[i]);
         spAddress += 4;
      }
   }

   rf.write(SP_REG, SP - 4 * numberOfRegisters - 4);
}

void popRegistersOffOfStack(MISC_Type misc) {
   unsigned int numberOfRegisters = countOneBits(misc.instr.pop.reg_list);
   unsigned int spAddress = SP;
   unsigned int i, mask;
   
   //pop it says to use 0 - 7
   for(i = 0, mask = 1; i < 8; i++, mask <<= 1) {
      if(misc.instr.pop.reg_list & mask) {
 
         rf.write(i, dmem[spAddress]);
         spAddress += 4;
      }
      /* PAGE 166, Do not think that it is needed but 
         including for reference later
        
        if registers<15> == '1' then
            LoadWritePC(MemA[address,4]);
      
      */
   }

   rf.write(SP_REG, SP + 4 * numberOfRegisters + 4);
   rf.write(PC_REG, LR);
}

// not sure how to handle overflow bit so ignoring for now.
void handleDPOps(DP_Ops dp_ops, DP_Type dp) {
   int result;
   /*if(dp_ops == DP_AND) {
      result = rf[dp.instr.dp.rnd] & rf[dp.instr.dp.m];
      rf.write(dp.instr.dp.rnd, result)
      updateFlags(results);
   }
   else if(dp_ops == DP_EOR) {
      result = rf[dp.instr.dp.rnd] ^ rf[dp.instr.dp.m];
      rf.write(dp.instr.dp.rnd, result)
      updateFlags(results);
   }
   else if(dp_ops == DP_LSL) {
      result = rf[dp.instr.dp.rnd] << (rf[dp.instr.dp.m] & 0xFF);
      rf.write(dp.instr.dp.rnd, result)
      updateFlags(results);
   }
   else if(dp_ops == DP_LSR) {
      result = rf[dp.instr.dp.rnd] >> (rf[dp.instr.dp.m] & 0xFF);
      rf.write(dp.instr.dp.rnd, result)
      updateFlags(results);
   }
   else if(dp_ops == DP_ASR) {
   }
   else if(dp_ops == DP_ADC) {
   }
   else if(dp_ops == DP_SBC) {
   }
   else if(dp_ops == D_ROR) {
   }
   else if(dp_ops == DP_TST) {
   }
   else if(dp_ops == DP_RSB) {
   }
   else*/ if(dp_ops == DP_CMP) {
      int arg1 = rf[dp.instr.dp.rdn];
      int arg2 = rf[dp.instr.dp.rm];
      result = arg1 - arg2;
      
      cout << arg1 << "   " << arg2 << endl; 
      
      if(result < 0) {
         flags.N = 1;
      }
      else {
         flags.N = 0;
      }
      
      if(result == 0) {
         flags.Z = 1;
      }
      else {
         flags.Z = 0;
      }
      //NOT SURE ABOUT OVERFLOW AND CARRY
      if(FALSE) {
         flags.C = 1;
      }
      else {
         flags.C = 0;
      }
      
      if(FALSE) {
         flags.V = 1;
      }
      else{
         flags.V = 0;
      }
   }
   /*else if(dp_ops == DP_CMN) {
   }
   else if(dp_ops == DP_ORR) {
   }
   else if(dp_ops == DP_MUL) {
   }
   else if(dp_ops == DP_BIC) {
   }
   else if(dp_ops == DP_MVN) {
   }
*/
   else {
      cout <<" ===== NEED TO IMPLEMENT SOME DP_TYPES ======\n";
   }
}

void execute() {
   Data16 instr = imem[PC];
   if(LOGGER) {
      cout << "\nPC: 0x" << setfill('0') << setw(8) << hex << PC << "    Instruction: 0x" << hex << instr.data_ushort() <<"\n";
      cout << "SP: " << SP  << "\n";
      cout << "LR: " << LR << "\n";
   }
      
   Thumb_Types itype;
   unsigned int pctarget = PC + 2;
   unsigned int addr;
   int diff, BitCount, bit;

   /* Convert instruction to correct type */
   ALU_Type alu(instr);
   SP_Type sp(instr);
   DP_Type dp(instr);
   LD_ST_Type ld_st(instr);
   MISC_Type misc(instr);
   COND_Type cond(instr);
   UNCOND_Type uncond(instr);
   LDM_Type ldm(instr);
   STM_Type stm(instr);
   LDRL_Type ldrl(instr);
   ADD_SP_Type addsp(instr);
   BL_Type blupper(instr);

   ALU_Ops add_ops;
   DP_Ops dp_ops;
   SP_Ops sp_ops;
   LD_ST_Ops ldst_ops;
   MISC_Ops misc_ops;
   BL_Ops bl_ops;

   rf.write(PC_REG, pctarget);

   itype = decode(ALL_Types(instr));
   switch(itype) {
      case ALU:
         add_ops = decode(alu);
         switch(add_ops) {
            case ALU_LSLI:
               //Stephen
               rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] + alu.instr.lsli.imm);
               break;
            case ALU_LSRI:
               //Stephen
               rf.write(alu.instr.lsri.rd, rf[alu.instr.lsri.rm] + alu.instr.lsri.imm);
               break;
            case ALU_ASRI:
               //Stephen
               rf.write(alu.instr.asri.rd, rf[alu.instr.asri.rm] + alu.instr.asri.imm);
               break;
            case ALU_ADDR:
               rf.write(alu.instr.addr.rd, rf[alu.instr.addr.rn] + rf[alu.instr.addr.rm]);
               break;
            case ALU_SUBR:
               //Stephen
               rf.write(alu.instr.subr.rd, rf[alu.instr.subr.rn] + rf[alu.instr.subr.rm]);
               break;
            case ALU_ADD3I:
               rf.write(alu.instr.add3i.rd, rf[alu.instr.add3i.rn] + alu.instr.add3i.imm);
               break;
            case ALU_SUB3I:
               //Stephen
               rf.write(alu.instr.sub3i.rd, rf[alu.instr.sub3i.rn] + alu.instr.sub3i.imm);
               break;
            case ALU_MOV:
               rf.write(alu.instr.mov.rdn, alu.instr.mov.imm);
               break;
            case ALU_CMP: 
               updateFlagsRegisterAndImmediateValue(alu);
               break;
            case ALU_ADD8I:
               rf.write(alu.instr.add8i.rdn, rf[alu.instr.add8i.rdn] + alu.instr.add8i.imm);
               break;
            case ALU_SUB8I:
               //Stephen
               rf.write(alu.instr.sub8i.rdn, rf[alu.instr.sub8i.rdn] + alu.instr.sub8i.imm);               
               break;
            default:
               break;
         }
         break;
      case DP:
         dp_ops = decode(dp);
         handleDPOps(dp_ops, dp);
         
      case SPECIAL:
         sp_ops = decode(sp);
         switch(sp_ops) {
            case SP_MOV:
               if (sp.instr.mov.d) {
                  rf.write(SP_REG, rf[sp.instr.mov.rm]);
               }
               else {
                  rf.write(sp.instr.mov.rd, rf[sp.instr.mov.rm]);
               }
               break;
         }
         break;
      case LD_ST:
         // You'll want to use these load and store models
         // to implement ldrb/strb, ldm/stm and push/pop
         ldst_ops = decode(ld_st);
         switch(ldst_ops) {
            case STRR:
               addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
               dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
               break;
            case LDRR:
               addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
               rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr]);
               break;
         }
         break;
      case MISC:
         misc_ops = decode(misc);
         switch(misc_ops) {
            case MISC_PUSH:
               pushRegistersOntoStack(misc);
               break;
            case MISC_POP:
               popRegistersOffOfStack(misc);
               break;
            case MISC_SUB:
               rf.write(SP_REG, SP - (misc.instr.sub.imm*4));
               break;
            case MISC_ADD:
               rf.write(SP_REG, SP + (misc.instr.add.imm*4));
               break;
         }
         break;
      case COND:
         decode(cond);
         // Once you've completed the checkCondition function,
         // this should work for all your conditional branches.
         if (checkCondition(cond.instr.b.cond)){
            rf.write(PC_REG, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2);
         }
         break;
      case UNCOND:
         decode(uncond);
         rf.write(PC_REG, PC + 2 * signExtend8to32ui(uncond.instr.b.imm) + 2);
         break;
      case LDM:
         //Stephen not sure and not need for fib
         decode(ldm);

         break;
      case STM:
      {   decode(stm);
         int counter = 0;
         for(int i = 0, mask = 1; i < 8; i++, mask <<= 1) {
            if(misc.instr.push.reg_list & mask) {
               dmem.write(rf[stm.instr.stm.rn] + counter, rf[i]);
               counter += 4;
            }
         }  
         // may not need to write back pg 175
         rf.write(stm.instr.stm.rn, stm.instr.stm.rn + 4 * countOneBits(stm.instr.stm.reg_list));
      }   break;
      case LDRL:
         //Stephen
         decode(ldrl);
         addr = SP + (ldrl.instr.ldrl.imm*4);
         rf.write(ldrl.instr.ldrl.rt, dmem[addr]);
         break;
      case ADD_SP:
         decode(addsp);
         rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
         break;
      case BL:
         bl_ops = decode(blupper);
         if (bl_ops == BL_UPPER) {
           // PC has already been incremented above
           Data16 instr2 = imem[PC];
           BL_Type bllower(instr2);
           if (blupper.instr.bl_upper.s) {
             addr = static_cast<int>(0xff<<24) | 
                    ((~(bllower.instr.bl_lower.j1 ^ blupper.instr.bl_upper.s))<<23) | 
                    ((~(bllower.instr.bl_lower.j2 ^ blupper.instr.bl_upper.s))<<22) | 
                    ((blupper.instr.bl_upper.imm10)<<12) | 
                    ((bllower.instr.bl_lower.imm11)<<1);
           }
           else { 
             addr = ((blupper.instr.bl_upper.imm10)<<12) | 
                    ((bllower.instr.bl_lower.imm11)<<1); 
           }
           // return address is 4-bytes away from the start of the BL insn 
           rf.write(LR_REG, PC + 2); 
           // Target address is also computed from that point 
           rf.write(PC_REG, PC + 2 + addr); 
         }
         else {
           cerr << "Bad BL format." << endl; exit(1); 
         }
       break; 
      default:
         cout << "[ERROR] Unknown Instruction to be executed" << endl;
         //exit(1);
         break;
   }
   
      if(LOGGER) {
         for(int i = 0; i < 16; i++) {
            cout << "Register " << i << ": " << rf[i]<< endl;
         }
      }
}
