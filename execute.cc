#include  <iostream>
#include  <iomanip>
#include "thumbsim.hpp"
#define PC_REG 15
#define LR_REG 14
#define SP_REG 13
#define PC rf[PC_REG]
#define LR rf[LR_REG]
#define SP rf[SP_REG]
#define LEFT_SHIFT 0
#define RIGHT_SHIFT 1
#define SUBTRACTION 2
#define ADDITION 3
#define LOGGER 0
#define TAKEN 0
#define NOT_TAKEN 1
int counter = 0;

ASPR flags;
enum OFType { OF_ADD, OF_SUB, OF_SHIFT };



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
      if (num1 >= num2) {
        flags.C = 1;
      }
      else if (((unsigned long long int)num1 - (unsigned long long int)num2) ==
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

unsigned int signExtend16to32ui(short i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend11to32ui(unsigned int i) {
   unsigned int mask = 1 << 10;

   
   if(mask & i) {
      i = i | 0xFFFFFC00;
   }
   else {
      i = i & 0x000003FF;
   }


return i;
   
}

void updateFlagsRegisterAndImmediateValueCMP(ALU_Type alu) {
   int regValue = rf[alu.instr.cmp.rdn];
   stats.numRegReads++;

   int immValue = alu.instr.cmp.imm;
   int difference = regValue - immValue;

   if(difference == 0) {
      flags.Z = 1;
   }
   else {
      flags.Z = 0;
   }
   if(difference < 0) {
      flags.N = 1;
   }
   else {
      flags.N = 0;
   }
   
   setCarryOverflow(regValue, immValue, OF_SUB);
}

void updateFlagsRegisterADDSUBRegisterImmmediate(ALU_Type alu, int operation, int flag) {
   // add and sub should have the same layout
   int regValue = flag == 3 ? rf[alu.instr.add3i.rn] : rf[alu.instr.add8i.rdn];
//    stats.numRegReads++;
   int immValue = flag == 3 ? alu.instr.add3i.imm : alu.instr.add8i.imm;
   int difference;
   
   if(operation == ADDITION) {
      difference = regValue + immValue;
   }
   else if(operation == SUBTRACTION) {
      difference = regValue - immValue;
   }

   if(difference == 0) {
      flags.Z = 1;
   }
   else {
      flags.Z = 0;
   }
   if(difference < 0) {
      flags.N = 1;
   }
   else {
      flags.N = 0;
   }
   
   if (operation == ADDITION) {
      setCarryOverflow(regValue, immValue, OF_ADD);
   }
   else if(operation == SUBTRACTION) {
      setCarryOverflow(regValue, immValue, OF_SUB);
   }
}

void updateFlagsRegisterADDSUBRegisterRegister(ALU_Type alu, int operation) {
   // add and sub should have the same layout
   int regValue = rf[alu.instr.addr.rn];
//    stats.numRegReads++;
   int regValue2 = rf[alu.instr.addr.rm];
//    stats.numRegReads++;
   int difference;
   
   if(operation == ADDITION) {
      difference = regValue + regValue2;
   }
   else if(operation == SUBTRACTION) {
      difference = regValue - regValue2;
   }

   if(difference == 0) {
      flags.Z = 1;
   }
   else {
      flags.Z = 0;
   }
   if(difference < 0) {
      flags.N = 1;
   }
   else {
      flags.N = 0;
   }
   
   if (operation == ADDITION) {
      setCarryOverflow(regValue, regValue2, OF_ADD);
   }
   else if(operation == SUBTRACTION) {
      setCarryOverflow(regValue, regValue2, OF_SUB);
   }
}

void updateFlagsRegisterAndImmediateValueLogicalShift(ALU_Type alu, int shiftType) {
   //lsl and lsr should have the same layout
   int regValue = rf[alu.instr.lsli.rm];
//    stats.numRegReads++;
   int immValue = alu.instr.lsli.imm;
   int difference;
   
   if(LEFT_SHIFT == shiftType) {
      difference = regValue << immValue;
   }
   else if(RIGHT_SHIFT == shiftType) {
      difference = regValue >> immValue;
   }
   else {
      cout << "GIVE ME GOOD DATA!!!\n";
   }

   if(difference == 0) {
      flags.Z = 1;
   }
   else {
      flags.Z = 0;
   }
   if(difference < 0) {
      flags.N = 1;
   }
   else {
      flags.N = 0;
   }
   
   setCarryOverflow(regValue, immValue, OF_SHIFT);
}

void updateFlagsMOV(int number) {
   int immValue = number;

   if(immValue == 0) {
      flags.Z = 1;
   }
   else {
      flags.Z = 0;
   }
   if(immValue < 0) {
      flags.N = 1;
   }
   else {
      flags.N = 0;
   }
}

void doBranchStatistics(unsigned int pc, unsigned int target, unsigned int takenFlag) {
   if(takenFlag == TAKEN) {
      if(target > pc) {
         stats.numForwardBranchesTaken++;
      }
      
      else {
         stats.numBackwardBranchesTaken++;
      }
   }
   else if(takenFlag == NOT_TAKEN) {
            if(target > pc) {
         stats.numForwardBranchesNotTaken++;
      }
      
      else {
         stats.numBackwardBranchesNotTaken++;
      }
   }
   else {
      cout <<"REALLLLLLY.......\n";
   } 

}

// You're given the code for evaluating BEQ, 
// and you'll need to fill in the rest of these.
// See Page 99 of the armv6 manual
//*good but did not really check*
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
   //might not be needed
   //instruction &= 0x00FF;
   int counter = 0;
   
   for( ; instruction; counter++) {
         instruction = instruction & (instruction - 1);
   }

   return counter;
}

void pushRegistersOntoStack(MISC_Type misc) {
   unsigned int numberOfRegisters = countOneBits(misc.instr.push.reg_list) + misc.instr.push.m;
   unsigned int spAddress = SP - 4 * numberOfRegisters;
//    stats.numRegReads++;
   unsigned int i, mask;
   
   /* pg167 I am using 8 because thumb has 
      8 registers in the list. The document
      says to use 14, so this may cause a problem 
      later.
    */
   for(i = 0, mask = 1; i < 8; i++, mask <<= 1) {
      if(misc.instr.push.reg_list & mask) {
         dmem.write(spAddress, rf[i]);
         stats.numRegReads++;
         stats.numMemWrites++;
         spAddress += 4;
      }
   }
   if(misc.instr.push.m) {
      dmem.write(spAddress, LR);
      stats.numMemWrites++;
   }
//    stats.numRegReads++;   
   rf.write(SP_REG, SP - 4 * numberOfRegisters);
   stats.numRegWrites++;
}

void popRegistersOffOfStack(MISC_Type misc) {
   unsigned int numberOfRegisters = countOneBits(misc.instr.pop.reg_list) + misc.instr.pop.m;
   unsigned int spAddress = SP;
//    stats.numRegReads++;
   unsigned int i, mask;
   
   //pop it says to use 0 - 7
   for(i = 0, mask = 1; i < 8; i++, mask <<= 1) {
      if(misc.instr.pop.reg_list & mask) {
         rf.write(i, dmem[spAddress]);
         stats.numRegWrites++;
         stats.numMemReads++;
         spAddress += 4;
      }
   }
   if(misc.instr.pop.m) {
      rf.write(PC_REG, dmem[spAddress]);
      stats.numMemReads++;
   }  
   rf.write(SP_REG, SP + 4 * numberOfRegisters);
   stats.numRegWrites++;
   stats.numRegWrites++;
}

//good
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
      stats.numRegReads++;
      int arg2 = rf[dp.instr.dp.rm];
      stats.numRegReads++;
      result = arg1 - arg2;
      
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
         setCarryOverflow(arg1, arg2, OF_SUB);
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
   stats.instrs++;

   Data16 instr = imem[PC];
//    stats.numRegReads++;
   caches.access((unsigned int)instr);
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
   stats.numRegWrites++;
   itype = decode(ALL_Types(instr));
   switch(itype) {
      case ALU:
         add_ops = decode(alu);
         switch(add_ops) {
            case ALU_LSLI:
               //Stephen
               updateFlagsRegisterAndImmediateValueLogicalShift(alu, LEFT_SHIFT);
               rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] << alu.instr.lsli.imm);
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_LSRI:
               //Stephen
               updateFlagsRegisterAndImmediateValueLogicalShift(alu, RIGHT_SHIFT);
               rf.write(alu.instr.lsri.rd, rf[alu.instr.lsri.rm] >> alu.instr.lsri.imm);
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_ASRI:
               //Stephen
               cout << "THIS IS MORE THAN LIKELY WRONG!\n";
               exit(0);
               rf.write(alu.instr.asri.rd, rf[alu.instr.asri.rm] >> alu.instr.asri.imm);
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_ADDR:
               //updateFlagsRegisterADDSUBRegisterRegister(alu, ADDITION);
               rf.write(alu.instr.addr.rd, rf[alu.instr.addr.rn] + rf[alu.instr.addr.rm]);
               stats.numRegReads++;
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_SUBR:
               //Stephen
               //updateFlagsRegisterADDSUBRegisterRegister(alu, ADDITION);
               rf.write(alu.instr.subr.rd, rf[alu.instr.subr.rn] - rf[alu.instr.subr.rm]);
               stats.numRegReads++;
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_ADD3I:
               updateFlagsRegisterADDSUBRegisterImmmediate(alu, ADDITION, 3);
               rf.write(alu.instr.add3i.rd, rf[alu.instr.add3i.rn] + alu.instr.add3i.imm);
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_SUB3I:
               //Stephen
               //updateFlagsRegisterADDSUBRegisterImmmediate(alu, SUBTRACTION, 3);
               rf.write(alu.instr.sub3i.rd, rf[alu.instr.sub3i.rn] - alu.instr.sub3i.imm);
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_MOV:
               updateFlagsMOV(alu.instr.mov.imm);
               rf.write(alu.instr.mov.rdn, alu.instr.mov.imm);
               stats.numRegWrites++;
               break;
            case ALU_CMP: 
               updateFlagsRegisterAndImmediateValueCMP(alu);
               break;
            case ALU_ADD8I:
               updateFlagsRegisterADDSUBRegisterImmmediate(alu, ADDITION, 8);
               rf.write(alu.instr.add8i.rdn, rf[alu.instr.add8i.rdn] + alu.instr.add8i.imm);
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case ALU_SUB8I:
               //Stephen
               updateFlagsRegisterADDSUBRegisterImmmediate(alu, SUBTRACTION, 8);
               rf.write(alu.instr.sub8i.rdn, rf[alu.instr.sub8i.rdn] - alu.instr.sub8i.imm); 
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            default:
               break;
         }
         break;
      case DP:
         dp_ops = decode(dp);
         handleDPOps(dp_ops, dp);
         break;
      case SPECIAL:
         sp_ops = decode(sp);
         switch(sp_ops) {
            case SP_MOV:
               if(sp.instr.mov.d == 1 && sp.instr.mov.d + 8 == 15) {
                  cout <<"PLEASE DONT EVER BE TRUE!!!\n";
               }
               else if (sp.instr.mov.d == 1) {
                 if (sp.instr.mov.rd + 8 == SP_REG) {
                   rf.write(SP_REG, rf[sp.instr.mov.rm]);                   
                 }
                 else {
                   rf.write(sp.instr.mov.rd + 8, rf[sp.instr.mov.rm]);
                 }
                  stats.numRegReads++;
                  stats.numRegWrites++;
               }
               else {
                  updateFlagsMOV(sp.instr.mov.rm);
                  rf.write(sp.instr.mov.rd, rf[sp.instr.mov.rm]);
                  stats.numRegReads++;
                  stats.numRegWrites++;
               }
               break;
            case SP_CMPR:
            {
               unsigned int dBit = 0;
               if(sp.instr.cmp.d == 1) {
                  dBit = 8;
               }
               int arg1 = rf[sp.instr.cmp.rd + dBit];
               stats.numRegReads++;
               int arg2 = rf[sp.instr.cmp.rm];
               stats.numRegReads++;
               int result = arg1 - arg2;

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
                  setCarryOverflow(arg1, arg2, OF_SUB);
            }
            break;
            case SP_ADD: 
              if (sp.instr.add.d == 1) {
                if (sp.instr.add.rd + 8 == SP_REG) { 
                  rf.write(SP_REG, SP + rf[sp.instr.add.rm]);
                }
                else {
                  rf.write(sp.instr.add.rd + 8, rf[sp.instr.add.rd + 8] + rf[sp.instr.add.rm]);
                }
              }
              else {
                rf.write(sp.instr.add.rd, rf[sp.instr.add.rd] + rf[sp.instr.add.rm]);
              }
               //rf.write(SP_REG, SP + rf[sp.instr.add.rm]);
               stats.numRegReads++;
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            default:
              cout << "MISSED A CASES\n";
         }
         break;
      case LD_ST:
         // You'll want to use these load and store models
         // to implement ldrb/strb, ldm/stm and push/pop
         ldst_ops = decode(ld_st);
         switch(ldst_ops) {
            case STRI:
               addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
               stats.numRegReads++;
               dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
               stats.numRegReads++;
               stats.numMemWrites++;
               break;
            case LDRI:
               addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
               stats.numRegReads++;
               rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr]);
               stats.numRegWrites++;
               stats.numMemReads++;
               break;
            case LDRBI: 
            {
               addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm;
               stats.numRegReads++;
               Data32 temp = dmem[addr];
               rf.write(ld_st.instr.ld_st_imm.rt, temp.data_ubyte4(0));
               stats.numMemReads++;
               stats.numRegWrites++;
             }
             break;
            case STRBI:
            {
               addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm;
               stats.numRegReads++;
               Data32 addr2 = dmem[addr];
               addr2.set_data_ubyte4(0, rf[ld_st.instr.ld_st_imm.rt] & 0x000000FF);
               stats.numRegReads++;
               dmem.write(addr, addr2);
//                stats.numMemReads++;
               stats.numMemWrites++;
            }                     
             break;
            case LDRBR:
	    {
               addr = rf[ld_st.instr.ld_st_reg.rn] + rf[ld_st.instr.ld_st_reg.rm];
               stats.numRegReads++;
               stats.numRegReads++;
               Data32 temp = dmem[addr];
               stats.numMemReads++;
               rf.write(ld_st.instr.ld_st_reg.rt, temp.data_ubyte4(0));
               stats.numRegWrites++;
            }   
            break;
            case STRBR:
            {
               addr = rf[ld_st.instr.ld_st_reg.rn] + rf[ld_st.instr.ld_st_reg.rm];
               stats.numRegReads++;
               stats.numRegReads++;
               
               Data32 addr2 = dmem[addr];
//                stats.numMemReads++;
               addr2.set_data_ubyte4(0, ((rf[ld_st.instr.ld_st_reg.rt] & 0x000000FF)));
               stats.numRegReads++;
               dmem.write(addr, addr2);
               stats.numMemWrites++;
               
               break;
            }
            break;
            case LDRR:
               addr = rf[ld_st.instr.ld_st_reg.rn] + rf[ld_st.instr.ld_st_reg.rm];
               stats.numRegReads++;
               stats.numRegReads++;
               rf.write(ld_st.instr.ld_st_reg.rt, dmem[addr]);
               stats.numMemReads++;
               stats.numRegWrites++;
               break;
            case STRR:
               addr = rf[ld_st.instr.ld_st_reg.rn] + rf[ld_st.instr.ld_st_reg.rm];
               stats.numRegReads++;
               stats.numRegReads++;
               dmem.write(addr, rf[ld_st.instr.ld_st_reg.rt]);
               stats.numRegReads++;
               stats.numMemWrites++;
               stats.numRegWrites++;
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
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
            case MISC_ADD:
               rf.write(SP_REG, SP + (misc.instr.add.imm*4));
               stats.numRegReads++;
               stats.numRegWrites++;
               break;
         }
         break;
      case COND:
         decode(cond);
         // Once you've completed the checkCondition function,
         // this should work for all your conditional branches.
         if (checkCondition(cond.instr.b.cond)){
            doBranchStatistics(PC, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2, TAKEN);
            rf.write(PC_REG, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2);
            stats.numRegWrites++;
         }
         else {
            doBranchStatistics(PC, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2, NOT_TAKEN);
         }
         stats.numRegReads++;
         break;
      case UNCOND:
         decode(uncond);
         rf.write(PC_REG, PC + 2 * signExtend11to32ui(uncond.instr.b.imm) + 2);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case LDM:
      {
         decode(ldm);
         
         int counter = 0;
         for(int i = 0, mask = 1; i < 8; i++, mask <<= 1) {
            if(ldm.instr.ldm.reg_list & mask) {
               rf.write(i, dmem[rf[ldm.instr.ldm.rn] + counter]);
               stats.numRegReads++;
               stats.numMemReads++;
               stats.numRegWrites++;
               counter += 4;
            }
         }  

         rf.write(ldm.instr.ldm.rn, rf[ldm.instr.ldm.rn] + 4 * countOneBits(ldm.instr.ldm.reg_list));
         stats.numRegReads++;
         stats.numRegWrites++;
       }  
         break;
      case STM:
      {   
         decode(stm);
         unsigned int addr = rf[stm.instr.stm.rn];
         stats.numRegReads++;
         int counter = 0;
         for(int i = 0, mask = 1; i < 8; i++, mask <<= 1) {
            if(stm.instr.stm.reg_list & mask) {
               dmem.write(addr + counter, rf[i]);
               stats.numRegReads++;
               counter += 4;
               stats.numMemWrites++;
            }
         }  
         // may not need to write back pg 175
         rf.write(stm.instr.stm.rn,  rf[stm.instr.stm.rn] + 4 * countOneBits(stm.instr.stm.reg_list));
         stats.numRegReads++;
         
         
      }   
      break;
         case LDRL:
      decode(ldrl);  
      {               
      // Need to check for alignment by 4
      if (PC & 2) {
        addr = PC + 2 + (ldrl.instr.ldrl.imm)*4;
      }
      else {
        addr = PC + (ldrl.instr.ldrl.imm)*4;
      }
      // Requires two consecutive imem locations pieced together
      Data32 temp = imem[addr] | (imem[addr+2]<<16);  // temp is a Data32
      rf.write(ldrl.instr.ldrl.rt, temp);
      
      // One write for updated reg
      stats.numRegWrites++;
      // One read of the PC
      stats.numRegReads++;
      // One mem read, even though it's imem, and there's two of them
      stats.numMemReads++;
      }
      break;
      case ADD_SP:
         decode(addsp);
         rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
         stats.numRegReads++;
         stats.numRegWrites++;
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
           stats.numRegReads++;
           stats.numRegWrites++;
           // Target address is also computed from that point 
           rf.write(PC_REG, PC + 2 + addr); 
           stats.numRegReads++;
           stats.numRegWrites++;
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
}
