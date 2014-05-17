#include  <iostream>
#include  <iomanip>
#include "thumbsim.hpp"
#define PC_REG 15
#define LR_REG 14
#define SP_REG 13
#define PC rf[PC_REG]
#define LR rf[LR_REG]
#define SP rf[SP_REG]
#define LOGGER 1

unsigned int signExtend16to32ui(short i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

ASPR flags;

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
         counter++;
   }

   return counter;
}

void pushRegistersOntoStack(MISC_Type misc) {
   unsigned int numberOfRegisters = countOneBits(misc.instr.push.reg_list);
   unsigned int spAddress = SP - 4 * numberOfRegisters;
   // I think we need to write the link register here!
   rf.write(LR_REG, SP);
   unsigned int i, mask;
   // why 14? pg167 I am using 8 because thumb has 
   // 8 registers in the list... hopefully
   for(i = 0, mask = 1; i < 8; i++, mask <<= 1) {
      if(misc.instr.push.reg_list & mask) {
         rf.write(spAddress, i);
         spAddress += 4;
      }
   }

   rf.write(SP_REG, SP - 4 * numberOfRegisters);
}

void popRegistersOffOfStack(MISC_Type misc) {
   unsigned int numberOfRegisters = countOneBits(misc.instr.pop.reg_list);
   unsigned int spAddress = SP;
   
   unsigned int i, mask;
   //pop it says to use 0 - 7
   for(i = 0, mask = 1; i < 8; i++, mask <<= 1) {
      if(misc.instr.pop.reg_list & mask) {
         // want to get the value at the address 
         rf.write(i, spAddress);
         spAddress += 4;
      }
      /* PAGE 166, Do not think that it is needed but 
         including for reference later
        
        if registers<15> == '1' then
            LoadWritePC(MemA[address,4]);
      
      */
   }

   rf.write(SP_REG, SP + 4 * numberOfRegisters);
}
void execute() {
   Data16 instr = imem[PC];
   if(LOGGER) {
      cout << "\nPC: 0x" << setfill('0') << setw(8) << hex << PC << "    Instruction: 0x" << hex << instr.data_ushort() <<"\n";
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

   ALU_Ops add_ops;
   DP_Ops dp_ops;
   SP_Ops sp_ops;
   LD_ST_Ops ldst_ops;
   MISC_Ops misc_ops;

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
               //Stephen NOT SURE HERE MATT
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
         decode(dp);
         break;
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
         break;
      case LDM:
         decode(ldm);
         break;
      case STM:
         decode(stm);
         break;
      case LDRL:
         //Stephen
         decode(ldrl);
         rf.write(ldrl.instr.ldrl.rt, SP + (ldrl.instr.ldrl.imm*4));
         break;
      case ADD_SP:
         decode(addsp);
         rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
         break;
      default:
         cout << "[ERROR] Unknown Instruction to be executed" << endl;
         exit(1);
         break;
   }
}
