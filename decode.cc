/**
 * Lab 5-6
 * CPE 315-01/02, Dr. Lupo
 * @author Matt Bague & Stephen Calabrese
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "decode.hpp"
#include <limits>

using namespace std;

static const unsigned int MAX_UINT = numeric_limits<unsigned int>::max();

int classify_type(const ALL_Types);
void printReg(unsigned int reg);

/* Prints load store immediate instruction */
void printLSimm(string op, const ALL_Types data) {
  cout << op << " ";
  printReg(data.type.ld_st.instr.ld_st_imm.rt);
  cout << ", [";
  printReg(data.type.ld_st.instr.ld_st_imm.rn);
  cout << ", #" << dec << data.type.ld_st.instr.ld_st_imm.imm * 4; 
  cout << "]" << endl;
}

/* Prints load store register instruction */
void printLSreg(string op, const ALL_Types data) {
  
}

/** Prints ALU immediate instruction 
 * @param op the string name of the instruction op
 * @param rd the destination register
 * @param rn the first register or -1 if only 2 values (e.g. cmp r1, #1 OR adds r1, #1)
 * @param imm the immediate value
 */

void printALUimm(string op, unsigned int rd, unsigned int rn, unsigned int imm) {
  cout << op << " ";
  printReg(rd); 
  if (rn != MAX_UINT) {
    cout << ", ";
    printReg(rn);
  }
  cout << ", #" << imm << endl; 
}

/* Prints ALU register instruction */
void printALUreg(string op, const ALL_Types data) {
  cout << op << " ";
  printReg(data.type.alu.instr.addr.rd); 
  cout << ", ";
  printReg(data.type.alu.instr.addr.rn); 
  cout << ", ";
  printReg(data.type.alu.instr.addr.rm);
  cout << endl;
}
void printRegList(unsigned int reg) {

   if (reg & 1) {
     cout << "r0, ";
   }
   if (reg & 2) {
     cout << "r1, ";
   }
   if (reg & 4) {
     cout << "r2, ";
   }
   if (reg & 8) {
     cout << "r3, ";
   }
   if (reg & 16) {
     cout << "r4, ";
   }
   if (reg & 32) {
     cout << "r5, ";
   }
   if (reg & 64) {
     cout << "r6, ";
   }
   if (reg & 128) {
     cout << "r7, ";
   }
}

void printReg(unsigned int reg) {
   cout << "r" << reg;
}

unsigned int getPCData(ifstream & in, unsigned int pc) {
  unsigned int addr, data;
  while ((in >> addr) && addr != pc) {
    in >> data;
  } 
  if (in >> data) {
//    cout << hex << addr << ":     ";
    return data;
  }
  else {
    cout << "BAD PC" << endl;
    exit(1);
  }

}

unsigned int getNextValidInstr(ifstream & in) {
  unsigned int addr, data;
  while (in >> addr >> data && data > 0x10000) {
  }
  if (addr != 0xda) {
 //   cout << hex << addr << ":     ";
  }
  return data;
}

Register pc;
Options opts;

int main(int argc, char ** argv) {
  unsigned int addr, data;
  int c;
  string s, filename;
  ALL_Types temp;
  while ((c = getopt(argc, argv, "pdiwsc:f:")) != -1) {
    switch(c) {
#if 0 // will include later in lab 6
      case 'p':
        opts.program = true;
        break;
      case 'd':
        opts.dump = true;
        break;
      case 'i':
        opts.instrs = true;
        break;
      case 'w':
        opts.writes = true;
        break;
      case 's':
        opts.stats = true;
        break;
      case 'c':
        //caches = Caches(atoi(optarg));
        break;
#endif
      case 'f':
        filename = optarg;
        break;
    }
  }

  if (filename == "") {
    cerr << "ERROR: no source file specified; "
      << "run this program with -f filename" << endl;
    exit(1);
  }  
  ifstream in(filename.c_str());
  in >> hex;                    // in takes hex numbers
  in >> s;
  if (s != "PC") {
    cout << "Invalid .sim file" << endl;
    exit(1);
  }
  in >> addr;
  pc = addr;
  in >> s;
  in >> s;
  cout << "Starting at PC " << hex << pc << endl;
  data = getPCData(in, pc);
  temp = ALL_Types(data);
  classify_type(temp);

  while(data = getNextValidInstr(in)) {
    temp = ALL_Types(data);
    if (classify_type(temp) == -1) 
      break;
  }
  return 0;
}

int classify_type(const ALL_Types data) {
   
   if (data.type.alu.instr.class_type.type_check == ALU_TYPE) {
       if (data.type.alu.instr.lsli.op == ALU_LSLI_OP) {
             cout << "add 1" << endl;
       }
       else if (data.type.alu.instr.lsri.op == ALU_LSRI_OP) {
             cout << "add 111" << endl;
       }
       else if (data.type.alu.instr.asri.op == ALU_ASRI_OP) {
             cout << "add 112" << endl;
       }
       else if (data.type.alu.instr.addr.op == ALU_ADDR_OP) { 
         printALUreg("adds", data);
       }
       else if (data.type.alu.instr.subr.op == ALU_SUBR_OP) {
        cout << "add 114" << endl;
       }
       else if (data.type.alu.instr.add3i.op == ALU_ADD3I_OP) {
          printALUimm("adds", data.type.alu.instr.add3i.rd, data.type.alu.instr.add3i.rn, data.type.alu.instr.add3i.imm);
       }
       else if (data.type.alu.instr.sub3i.op == ALU_SUB3I_OP) {
         cout << "add 116" << endl;
       }
       else if (data.type.alu.instr.add8i.op == ALU_ADD8I_OP) {
         printALUimm("adds", data.type.alu.instr.add8i.rd, -1, data.type.alu.instr.add8i.imm);
       }
       else if (data.type.alu.instr.sub8i.op == ALU_SUB8I_OP) {
         cout << "add 118" << endl;
       }
       else if (data.type.alu.instr.cmp.op == ALU_CMP_OP) {
        printALUimm("cmp", data.type.alu.instr.cmp.rd, -1, data.type.alu.instr.cmp.imm);
       }
      else if (data.type.alu.instr.mov.op == ALU_MOV_OP) {
         if(data.type.alu.instr.mov.op == 0x4) {
            cout << "movs ";
            printReg(data.type.alu.instr.mov.rd);
            cout << ", #" << data.type.alu.instr.mov.imm << endl; 
         }
         else if(data.type.alu.instr.mov.op == 0x8) {
            cout << "movs ";
            printReg(data.type.alu.instr.mov.imm & 0x7);
            printReg((data.type.alu.instr.mov.imm & 0x78) >> 3);
         }
         else if(data.type.alu.instr.mov.op == 0x0) {
            cout << "mov ";
            printReg(data.type.alu.instr.mov.imm & 0x7);
            printReg((data.type.alu.instr.mov.imm & 0x38) >> 3);
         }
         else {
            cout << "REMOVE SHOULD NOT BE HERE" << endl;
         }
      }
      else {
         cout << "COULD NOT FIND: ALU_TYPE" << endl;
      }
      return ALU_TYPE;
   }
   else if (data.type.dp.instr.class_type.type_check == DP_TYPE) {
      cout << "add 121" << endl;
      return DP_TYPE;
   }
   else if (data.type.sp.instr.class_type.type_check == SP_TYPE) {
      cout << "mov sp, ";
      printReg((data.type.sp.instr.class_type.data & 0x78) >> 3);
      cout << "\n";
      
      
      return SP_TYPE;
   }
   else if (data.type.uncond.instr.class_type.type_check == UNCOND_TYPE) {
      cout << "b 0x";
      cout << hex << data.type.uncond.instr.class_type.data << endl;  // MATT THESE PRINT IN HEX???
      
      return UNCOND_TYPE;
   }
   else if (data.type.misc.instr.class_type.type_check == MISC_TYPE) {
      // There are more than these two MISC_TYPES,
      // You'll need to complete them here.
      if (data.type.misc.instr.push.op == MISC_PUSH_OP) {
         cout << "push ";
         cout << "{";
         printRegList(data.type.misc.instr.push.reg_list);
         cout << "lr}" << endl;
      }
      else if (data.type.misc.instr.sub.op == MISC_SUB_OP) {
         cout << "sub sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.type.misc.instr.sub.imm)<< 2) << endl;
      }
      else if(data.type.misc.instr.pop.op == MISC_POP_OP) {
         cout << "pop ";
         cout << "{";
         printRegList(data.type.misc.instr.pop.reg_list);
         cout << "pc}" << "\n";
      }
      else if(data.type.misc.instr.add.op == MISC_ADD_OP) {
         cout << "add sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.type.misc.instr.add.imm)<< 2) << endl;
      }
      else {
         cout << "COULD NOT FIND: MISC_TYPE" << endl;
      }
      
      return MISC_TYPE;
   }
   // Complete the rest of these
   else if (data.type.cond.instr.class_type.type_check == COND_TYPE) {
      cout << "b";
      ALL_Types::printCond(data.type.cond.instr.b.cond);
      cout << " 0x" << hex << data.type.cond.instr.b.imm << endl;   
       
      return COND_TYPE;
   }
   else if (data.type.ldm.instr.class_type.type_check == LDM_TYPE) {
      cout << "add 12";
      return LDM_TYPE;
   }
   else if (data.type.stm.instr.class_type.type_check == STM_TYPE) {
      cout << "add 13";
      return STM_TYPE;
   }
   else if (data.type.ldrl.instr.class_type.type_check == LDRL_TYPE) {
      cout << "add 14";
      return LDRL_TYPE;
   }
   else if (data.type.addsp.instr.class_type.type_check == ADD_SP_TYPE) {
      cout << "add ";
     
      if(data.type.addsp.instr.add.type_check == 0x15) {
         printReg(data.type.addsp.instr.add.rd);
         cout << ", sp, ";
         cout << "#" << data.type.addsp.instr.add.imm << endl;
      }
      else if(data.type.addsp.instr.add.type_check == 0x16) {
         cout << ", sp, sp, #" << data.type.addsp.instr.add.imm << endl;
      }
      else {
         cout << "COULD NOT FIND: ADD_SP_TYPE" << endl;
      }

      return ADD_SP_TYPE;
   }
   else {
      // Complete the rest of these instruction classes
      if (data.type.ld_st.instr.class_type.opA == LD_ST_REG_OPA) {
          cout << "add 16";
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMM_OPA) {
         if (data.type.ld_st.instr.class_type.opB == LD_ST_OPB_STR) {
            printLSimm("str", data);
         }
         else if (data.type.ld_st.instr.class_type.opB == LD_ST_OPB_LDR) {
            printLSimm("ldr", data);
         }
         else {
            cout << "COULD NOT FIND: LD_ST_IMM_OPA" << endl;
         }
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMB_OPA) {
         cout << "add 19";
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMH_OPA) {
         cout << "add 20";
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMSP_OPA) {
         cout << "add 21";
      }
      else {
         cout << "NOT A VALID INSTRUCTION" << endl;
      }
   
      return 0;
  }
  cout << endl << "NO TYPE FOUND" << endl;
  return -1; // Error
}
