// Paolo Ratti Tamayo CS350-01

// CS 350, Spring 2016
// Final Project part 2
//
// Illinois Institute of Technology, (c) 2016, James Sasaki

#include <stdio.h>
#include <stdlib.h>       // For error exit()

// CPU Declarations -- a CPU is a structure with fields for the
// different parts of the CPU.
//
    typedef short int Word;          // type that represents a word of SDC memory
    typedef unsigned char Address;   // type that represents an SDC address

    #define MEMLEN 65535
    #define NREG 8

    typedef struct {
        Word mem[MEMLEN];
        Word reg[NREG];      // Note: "register" is a reserved word
        Address pc;          // Program Counter
        int running;         // running = 1 iff CPU is executing instructions
        Word ir;             // Instruction Register
        int instr_sign;      //   sign of instruction
        int opcode;          //   opcode field
        int reg_R;           //   register field
        int addr_MM;		//   memory field
		int orig; 			//	 to get the .ORIG location
		int counter;   		// counter
		int f;				// counter 2 //note changed, almost unused
		char CC;		// Condition Code  	
    } CPU;


// Prototypes [note the functions are also declared in this order]
//
    // *** STUB *** Lab 6 items omitted ....
	 int main(int argc, char *argv[]);
    void initialize_CPU(CPU *cpu);
    void initialize_memory(int argc, char *argv[], CPU *cpu);
    FILE *get_datafile(int argc, char *argv[]);

    void dump_CPU(CPU *cpu);
    void dump_memory(CPU *cpu,int loc);
    void dump_registers(CPU *cpu);
//  //part 2 
    int read_execute_command(CPU *cpu);
    int execute_command(char cmd_char, CPU *cpu);
    void help_message(void);
    void many_instruction_cycles(int nbr_cycles, CPU *cpu);
    void one_instruction_cycle(CPU *cpu);

    void exec_HLT(CPU *cpu);
    // *** STUB ***
	void changeCC(CPU *cpu, int reg);
// Main program: Initialize the cpu, and read the initial memory values
//
int main(int argc, char *argv[]) {
    printf("CS 350 Final Project part 2\nPaolo Ratti Tamayo CS350-01");
    CPU cpu_value, *cpu = &cpu_value;
  
	initialize_CPU(cpu);
    //printf("Works\n");
    initialize_memory(argc, argv, cpu);
    char *prompt = "> ";
    printf("\nBeginning execution; type h for help\n%s", prompt);
	
    int done = read_execute_command(cpu);
    while (!done) {
        printf("%s", prompt);
        done = read_execute_command(cpu);
    }
    return 0;
}


// *** STUB ***
// Lab 6 items omitted ....
//    initialize_CPU, ..., dump registers
void initialize_CPU(CPU *cpu) {
    
    // *** STUB ***
  int i = 0;
  cpu -> pc = 0;
  cpu -> ir = 0;
  cpu -> instr_sign = 1;
  cpu -> running = 1;
  cpu -> reg_R = 0;
  cpu -> opcode = 0;
  cpu -> orig = 0;
  cpu -> f = 0;
  cpu -> counter = 0;
  cpu -> CC = 'Z';
  for (i = 0; i<MEMLEN; i++)
    {
      cpu -> mem[i] = 0;
    }
  
  for (i = 0; i<NREG; i++)
    {
      cpu -> reg[i] = 0;
    }
  
  //cpu -> mem[] = NULL;
  //cpu -> reg[NREG] = NULL;
  printf("\nInitial CPU:\n");
  dump_CPU(cpu);
  printf("\n");
}

// reading from the file
void initialize_memory(int argc, char *argv[], CPU *cpu) {
    FILE *datafile = get_datafile(argc, argv);

// Buffer to read next line of text into
#define BUFFER_LEN 80
    char buffer[BUFFER_LEN];

    // Will read the next line (words_read = 1 if it started
    // with a memory value). Will set memory location loc to
    // value_read
    //
    int value_read, words_read, loc = 0, done = 0;

    char *read_success;    // NULL if reading in a line fails.
    
    read_success = fgets(buffer, BUFFER_LEN, datafile);
    while (read_success != NULL && done!=1) {
        // If the line of input begins with an integer, treat
        // it as the memory value to read in.  Ignore junk
        // after the number and ignore blank lines and lines
        // that don't begin with a number.
        //
        words_read = sscanf(buffer, "%x", &value_read); //if there is an integer assigns words_read to 1, else 0

        // *** STUB *** set memory value at current location to
        // value_read and increment location.  Exceptions: If
        // loc is out of range, complain and quit the loop. If
        // value_read is outside -9999...9999, then it's a
        // sentinel and we should say so and quit the loop.
		if (words_read==1)
		{
			if(loc == MEMLEN) //if the maximum limit for loc has been reached, it returns done = 1, which ends the loop
			  {
				printf("location is out of range.location is %d\n", loc);
				done = 1;
			  }
			if (-65535<=value_read && value_read<=65535)
			{
				cpu -> mem[loc] = value_read;
				loc++;
			}
			else 
			{
			// sentinel commented
			// if (value_read>9999 || value_read<-9999) //if the value read is outside the range, it returns done = 1, which ends the loop
			  // {
				
				// printf("Sentinel %d found at loc %d\n",value_read, loc);
				// cpu -> mem[loc] = value_read;
				// loc++;
				// done = 1;
			  // }
				
			
			}
		}
		
        // Get next line and continue the loop
        //
        // *** STUB ***
	read_success = fgets(buffer, BUFFER_LEN, datafile);
    }
    (*cpu).pc = (*cpu).mem[(*cpu).pc];
	(*cpu).ir = 0;
	(*cpu).running = 1;
    // Initialize rest of memory
    //
	int loc2 = loc;
    while (loc < MEMLEN) 
	{
        cpu -> mem[loc++] = 0;
    }
   
    dump_memory(cpu,loc2);
}

FILE *get_datafile(int argc, char *argv[]) {
    char *default_datafile_name = "default.hex";
    char *datafile_name = NULL;
	
    // *** STUB *** set datafile name to argv[1] or default
    if (argv[1] == NULL)
	{
		datafile_name = default_datafile_name; 
		printf("Using data from %s\n", datafile_name); 
	}
	else
	{
		datafile_name = argv [1];
		printf("Using data from %s\n",argv [1]);
	}
		
	
    FILE *datafile = fopen(datafile_name, "r");
	if (datafile_name==NULL)
	{
		printf("Could not open the file.");
		exit(EXIT_FAILURE);
	}
   

    return datafile;
}
// Read a simulator command from the keyboard (q, h, ?, d, number,
// or empty line) and execute it.  Return true if we hit end-of-input
// or execute_command told us to quit.  Otherwise return false.
//
int read_execute_command(CPU *cpu) {
// Buffer to read next command line into
#define CMD_LINE_LEN 80
    char cmd_line[CMD_LINE_LEN];
    char *read_success;     // NULL if reading in a line fails.

    int nbr_cycles;     // Number of instruction cycles to execute
    char cmd_char;      // Command 'q', 'h', '?', 'd', or '\n'
    int done = 0;       // Should simulator stop?

    read_success = fgets(cmd_line, CMD_LINE_LEN, stdin);

    // *** STUB ***  Quit if read_success indicates end-of-file
	if (read_success == NULL)
	{
		exit(0); 
	}
	else 
	{
		int iread;
		iread = sscanf(cmd_line, "%d", &nbr_cycles);
		if (iread ==1)
		{
			many_instruction_cycles(nbr_cycles, cpu);
		}
		else 
		{
			iread = sscanf(cmd_line, "%c", &cmd_char);
			done = execute_command(cmd_char, cpu);
		}
	}
    // *** STUB ***
    // while not done,
    //      Handle q, h, ?, d commands, integer (nbr of instruction cycles),
    //          or empty line (one instruction cycle)
    //      Read next command line

    return done;
}

// Execute a nonnumeric command; complain if it's not 'h', '?', 'd', 'q' or '\n'
// Return true for the q command, false otherwise
//
int execute_command(char cmd_char, CPU *cpu) {
    if (cmd_char == '?' || cmd_char == 'h')
    {
        help_message();
    }
    else  if (cmd_char == 'q')
    {
       	printf("Quitting\n");
       	exit(0);
    }
    else if (cmd_char == 'd')
    {
      dump_CPU(cpu);
      dump_memory(cpu,MEMLEN);
    }
    else if (cmd_char == '\n')
      {
	one_instruction_cycle(cpu);
      }
    else
      {
	printf("Unknown command\n");
      }     
    return 0;
}

// Print standard message for simulator help command ('h' or '?')
//
void help_message(void) 
{
	printf("Simulator commands: \n");
	printf("h or ? for help (prints this message)\n");
	printf("d to dump the CPU\n");
	printf("An integer > 0 to execute that many instruction cycles\n");
	printf("Or just return, which executes one instruction cycle\n\n");
	
	printf("SDC Instruction set:\n");
	printf(" 0xxx: HALT");
	printf(" 1RMM: Load Reg[R] <- M[MM]\n");
	printf(" 2RMM: Store M[MM] <- Reg[R]\n");
	printf(" 3RMM: Add M[MM] to Reg[R]\n");
	printf(" 4Rxx: Negate Reg[R]\n");
	printf(" 5RMM: Load Immediate Reg <- MM\n");
	printf("-5RMM: Load Immediate Reg <- -MM\n");
	printf(" 6RMM: Add Immediate Reg[R] <- Reg[R] +MM\n");
	printf("-6RMM: Substract immediate Reg[R] <- Reg[R] - MM\n");
	printf(" 7xMM: Branch to MM\n");
	printf(" 8RMM: Branch Conditional to MM\n");
	printf(" 90xx: Read char into R0\n");
	printf(" 91xx: Printchar in R0\n");
	printf(" 92MM: Print string starting at MM\n");
	printf(" 93MM: Dump CPU\n");
	printf(" 94MM: Dump memory\n");
	
}

// Execute a number of instruction cycles.  Exceptions: If the
// number of cycles is <= 0, complain and return; if the CPU is
// not running, say so and return; if the number of cycles is
// insanely large, complain and substitute a saner limit.
//
// If, as we execute the many cycles, the CPU stops running,
// stop and return.
//
void many_instruction_cycles(int nbr_cycles, CPU *cpu) {
    int INSANE_NBR_CYCLES = 100;

    // *** STUB ****
	if (nbr_cycles > INSANE_NBR_CYCLES)
	{
		printf("int is to large too use, changing it to a sane limit.\n");
		nbr_cycles = INSANE_NBR_CYCLES;
		for (int i = 0; i<nbr_cycles; i++)
		{
			one_instruction_cycle(cpu);
		}
	}
	else if (nbr_cycles < 1)
	{
		printf("int is too small, executing next instruction.\n");
		one_instruction_cycle(cpu);
	}
	else
	{
		for (int i = 0; i<nbr_cycles; i++)
		{
			one_instruction_cycle(cpu);
		}
	}
}
void changeCC(CPU *cpu, int reg)
{
	if (reg < 0)
	{
		(*cpu).CC = 'N';
	}
	else if (reg ==0)
	{
		(*cpu).CC = 'Z';
	}
	else 
	{
		(*cpu).CC = 'P';
	}
}
// Execute one instruction cycle
//
void one_instruction_cycle(CPU *cpu) {
    // If the CPU isn't running, say so and return.
    // If the pc is out of range, complain and stop running the CPU.
    //
    // *** STUB ****
	
	if ((*cpu).running != 1)
	{
		printf("EXECUTION HALTED\n");
	}
	
	else {
		int pos = (1<<16)-1;
		if ((*cpu).orig==0) //getting starting position in counter
		{
			(*cpu).counter = (*cpu).mem[0]; //x8000
			//(*cpu).counter++;
		}
		
		if ((*cpu).counter < 0) // if counter is negative
		{
			int po = (1<<16)-1;
			(*cpu).counter = (*cpu).counter&po;
		}
		
		
		//int PC = 0;
		if ((*cpu).orig==0) //getting starting position with pc
		{
			(*cpu).pc = (*cpu).mem[0]&pos;
			//printf("\n \n \n \n %x\n \n \n ",(*cpu).pc);
		}
		
		if ((*cpu).orig==0) //assigns every mem to its place in (*c)
		{
			//int a = 0; // for R0 storage purposes
			int tempCount = (*cpu).counter; //x8000 
			for (int o = 1; o<200; o++)
			{
				(*cpu).mem[tempCount] = (*cpu).mem[o]; //gives to mem at x8000
				//printf("\nx%x\n",(*cpu).mem[(*cpu).counter]);
				(*cpu).mem[o] = 0;
				tempCount++;
			} 
			(*cpu).mem[0] = 0;
			(*cpu).orig++;
		}	
		//skips if the instruction is 0
		if ((*cpu).mem[(*cpu).counter]==0)
		{
			(*cpu).f++;
			(*cpu).counter = (*cpu).counter + 1;
		}
		else 
		{
			//print cpu counter 
			if ((*cpu).mem[(*cpu).counter]<0)
			{	
				printf("x%04x:	x%04x	", (*cpu).counter, (*cpu).mem[(*cpu).counter] & pos);
				
			}
			else
			{
				printf("x%04x:	x%04x	", (*cpu).counter, (*cpu).mem[(*cpu).counter]& pos);
			}
			//variables
			int opcode;
			int reg_1;
			int reg_2;
			int reg_3;
			int flag;
			int offset;
			int val; // for the condition code
			
			if ((*cpu).mem[(*cpu).counter]<0)
			{					
				opcode = (((*cpu).mem[(*cpu).counter]&pos)>>12)&15;
				reg_1 = (((*cpu).mem[(*cpu).counter]&pos)>>9)&7;
			} 
			else
			{
				opcode = (((*cpu).mem[(*cpu).counter])>>12)&15;
				reg_1 = (((*cpu).mem[(*cpu).counter])>>9)&7;
			}
			
			switch (opcode)
			{
				case 0: 
						if ((*cpu).CC=='Z')
							val = 2;
						else if ((*cpu).CC=='P')
							val = 1;
						else 
							val = 4;
						offset = (*cpu).mem[(*cpu).counter]&511;
						(*cpu).counter++;
						if (reg_1 == 0)
						{
							printf("NOP	%d			; CC = %c, no branch\n", offset,(*cpu).CC);
						}
						else if (reg_1 == 1)
						{
							val = val&reg_1;
							printf("BRP	%d			", offset);
							if (val!=0)
							{
								printf("; PC <- x%x + %d = x%x		; CC = %c\n",(*cpu).counter, offset, (*cpu).counter+offset,(*cpu).CC);
								(*cpu).counter = (*cpu).counter+offset;
							}
							else 
								printf("; CC = %c, no branch\n",(*cpu).CC);
						}
						else if (reg_1 == 2)
						{
							val = val&reg_1;
							printf("BRZ	%d			", offset);
							if (val!=0)
							{
								printf("; PC <- x%x + %d = x%x		; CC = %c\n",(*cpu).counter, offset, (*cpu).counter+offset,(*cpu).CC);
								(*cpu).counter = (*cpu).counter+offset;
							}
							else 
								printf("; CC = %c, no branch\n",(*cpu).CC);
						}
						else if (reg_1 == 3)
						{
							val = val&reg_1;
							printf("BRZP	%d	", offset);
							if (val!=0)
							{
								printf("; PC <- x%x + %d = x%x		; CC = %c\n",(*cpu).counter, offset, (*cpu).counter+offset,(*cpu).CC);
								(*cpu).counter = (*cpu).counter+offset;
							}
							else 
								printf("; CC = %c, no branch\n",(*cpu).CC);
						}
						else if (reg_1 == 4)
						{
							val = val&reg_1;
							printf("BRN	%d			", offset);
							if (val!=0)
							{
								printf("; PC <- x%x + %d = x%x		; CC = %c\n",(*cpu).counter, offset, (*cpu).counter+offset,(*cpu).CC);
								(*cpu).counter = (*cpu).counter+offset;
							}
							else 
								printf("; CC = %c, no branch\n",(*cpu).CC);
						}
						else if (reg_1 == 5)
						{
							val = val&reg_1;
							printf("BRNP	%d			", offset);
							if (val!=0)
							{
								printf("; PC <- x%x + %d = x%x		; CC = %c\n",(*cpu).counter, offset, (*cpu).counter+offset,(*cpu).CC);
								(*cpu).counter = (*cpu).counter+offset;
							}
							else 
								printf("; CC = %c, no branch\n",(*cpu).CC);
						}
						else if (reg_1 == 6)
						{
							val = val&reg_1;
							printf("BRNZ	%d			", offset);
							if (val!=0)
							{
								printf("; PC <- x%x + %d = x%x		; CC = %c\n",(*cpu).counter, offset, (*cpu).counter+offset,(*cpu).CC);
								(*cpu).counter = (*cpu).counter+offset;
							}
							else 
								printf("; CC = %c, no branch\n",(*cpu).CC);
						}
						else if (reg_1 == 7)
						{
							val = val&reg_1;
							printf("BR	%d			", offset);
							if (val!=0)
							{
								printf("; PC <- x%x + %d = x%x		; CC = %c\n",(*cpu).counter, offset, (*cpu).counter+offset,(*cpu).CC);
								(*cpu).counter = (*cpu).counter+offset;
							}
							else 
								printf("; CC = %c, no branch\n",(*cpu).CC);
						}
						
						break;
				case 1:
						reg_2 = (((*cpu).mem[(*cpu).counter])>>6)&7;
						flag = (((*cpu).mem[(*cpu).counter])>>5)&1;
						reg_3 = (*cpu).mem[(*cpu).counter]&7;
						offset = (*cpu).mem[(*cpu).counter]&31;
						(*cpu).counter++;
						//printf("%d\n",flag);
						if (flag==0)
						{
							changeCC(cpu, ((*cpu).reg[reg_2]+(*cpu).reg[reg_3])&pos);
							printf("ADD	R%x, R%x, R%x		; R%x <- x%x + x%x = x%x		; CC = %c\n",reg_1,reg_2,reg_3,reg_1,(*cpu).reg[reg_2]&pos,(*cpu).reg[reg_3]&pos,((*cpu).reg[reg_2]+(*cpu).reg[reg_3])&pos, (*cpu).CC);
							(*cpu).reg[reg_1] = (*cpu).reg[reg_2]+(*cpu).reg[reg_3];
						}
						else
						{
							changeCC(cpu, ((*cpu).reg[reg_2]+offset)&pos);
							printf("ADD	R%x, R%x, %d 		; R%x <- x%x + %d = x%x		; CC = %c\n",reg_1, reg_2, offset, reg_1, (*cpu).reg[reg_2]&pos, offset, ((*cpu).reg[reg_2]+offset)&pos, (*cpu).CC);
							(*cpu).reg[reg_1] = (*cpu).reg[reg_2]+offset;
						}
						break;
				case 2: 
						offset = (*cpu).mem[(*cpu).counter]&511;
						(*cpu).counter++;
						changeCC(cpu, ((*cpu).mem[(*cpu).counter+offset])&pos);
						printf("LD	R%x, %09d		; R%x <- M[x%x] = x%x		; CC = %c\n", reg_1, offset, reg_1, (*cpu).counter+offset, (*cpu).mem[(*cpu).counter+offset]&pos, (*cpu).CC);
						(*cpu).reg[reg_1] = (*cpu).mem[(*cpu).counter+offset];
						break;
				case 3:
						offset = (*cpu).mem[(*cpu).counter]&511;
						(*cpu).counter++;
						printf("ST	R%x, %09d		; M[x%x] <- R%x\n", reg_1, offset, (*cpu).counter+offset, reg_1);
						(*cpu).mem[(*cpu).counter+offset] = (*cpu).reg[reg_1];
						break;
				case 4: 
						reg_2 = (((*cpu).mem[(*cpu).counter])>>6)&7;
						flag = (((*cpu).mem[(*cpu).counter])>>11)&1;
						offset = (*cpu).mem[(*cpu).counter]&2047;
						(*cpu).counter++;
						if (flag == 1)
						{
							printf("JSR	%011d		; R7 <- x%x	; PC <- x%x + %011d = x%x\n",offset, (*cpu).counter&pos, (*cpu).counter&pos, offset, (*cpu).counter + offset);
							(*cpu).reg[7] = (*cpu).counter;
							(*cpu).counter = (*cpu).counter + offset;
						}
						else
						{
							printf("JSRR	R%x			; R7 <- x%x	; PC <- R%x = x%x\n",reg_2, (*cpu).counter, reg_2, (*cpu).reg[reg_2]);
							(*cpu).reg[7] = (*cpu).counter;
							(*cpu).counter = (*cpu).reg[reg_2];
						}
						
						break;
				case 5:
						reg_2 = (((*cpu).mem[(*cpu).counter])>>6)&7;
						flag = (((*cpu).mem[(*cpu).counter])>>5)&1;
						reg_3 = (*cpu).mem[(*cpu).counter]&7;
						offset = (*cpu).mem[(*cpu).counter]&31;
						(*cpu).counter++;
						//printf("%d\n",flag);
						if (flag==0)
						{
							changeCC(cpu, ((*cpu).reg[reg_2] & (*cpu).reg[reg_3])&pos);
							printf("AND	R%x, R%x, R%x		; R%x <- x%x & x%x = x%x		; CC = %c\n",reg_1,reg_2,reg_3,reg_1,(*cpu).reg[reg_2]&pos,(*cpu).reg[reg_3]&pos,((*cpu).reg[reg_2]&(*cpu).reg[reg_3]),(*cpu).CC);
							(*cpu).reg[reg_1] = (*cpu).reg[reg_2] & (*cpu).reg[reg_3];
						}
						else
						{
							changeCC(cpu, ((*cpu).reg[reg_2] & offset)&pos);
							printf("AND	R%x, R%x, %d		; R%x <- x%x & x%x = x%x		; CC = %c\n", reg_1, reg_2, offset, reg_1,(*cpu).reg[reg_2]&pos,offset,((*cpu).reg[reg_2]&offset)&pos,(*cpu).CC);
							(*cpu).reg[reg_1] = (*cpu).reg[reg_2] & offset;
						}
						break;
				case 6:
						reg_2 = (((*cpu).mem[(*cpu).counter])>>6)&7;
						offset = (*cpu).mem[(*cpu).counter]&63;
						(*cpu).counter++;
						changeCC(cpu, ((*cpu).mem[(*cpu).reg[reg_2]+offset])&pos);
						printf("LDR	R%x, R%x, %06d		; R%x <- M[x%x] = x%x		; CC = %c\n",reg_1, reg_2, offset, reg_1, ((*cpu).reg[reg_2]+offset)&pos, (*cpu).mem[(*cpu).reg[reg_2]+offset]&pos,(*cpu).CC);
						(*cpu).reg[reg_1] = (*cpu).mem[(*cpu).reg[reg_2]+offset];
						break;
				case 7:
						reg_2 = (((*cpu).mem[(*cpu).counter])>>6)&7;
						offset = (*cpu).mem[(*cpu).counter]&63;
						(*cpu).counter++;
						printf("STR	R%x, R%x, %06d		; M[x%x] <- R%x\n",reg_1, reg_2, offset, ((*cpu).reg[reg_2]+offset)&pos, reg_1);
						(*cpu).mem[(*cpu).reg[reg_2]+offset] = (*cpu).reg[reg_1];
						break;
				case 8: 
						(*cpu).counter++;
						printf("RTI  				; Reserved opcode; ignored\n");
						break;
				case 9: 
						reg_2 = (((*cpu).mem[(*cpu).counter])>>6)&7;
						(*cpu).counter++;
						changeCC(cpu, ((*cpu).reg[reg_2]*-1)&pos);
						printf("NOT	R%x, R%x			; R%x <- NOT R%x = NOT %d = %d		; CC = %c\n",reg_1, reg_2, reg_1, reg_2, (*cpu).reg[reg_2], ((*cpu).reg[reg_2]*-1),(*cpu).CC);
						(*cpu).reg[reg_1] = ((*cpu).reg[reg_2]*-1);
						break;
				case 10:
						offset = (*cpu).mem[(*cpu).counter]&511;
						(*cpu).counter++;
						changeCC(cpu, ((*cpu).mem[(*cpu).mem[(*cpu).counter+offset]])&pos);
						printf("LDI	R%x, %09d		; R%x <- M[M[x%x]] = M[x%x] = x%x		; CC = %c\n",reg_1,offset, reg_1,(*cpu).counter+offset,(*cpu).mem[(*cpu).counter+offset]&pos,(*cpu).mem[(*cpu).mem[(*cpu).counter+offset]]&pos,(*cpu).CC);
						(*cpu).reg[reg_1] = (*cpu).mem[(*cpu).mem[(*cpu).counter+offset]];
						break;
				case 11:
						offset = (*cpu).mem[(*cpu).counter]&511;
						(*cpu).counter++;
						printf("STI	R%x, %09d		; M[M[x%x]] = M[x%x] <- R%x = x%x\n",reg_1,offset,(*cpu).counter+offset,(*cpu).mem[(*cpu).counter+offset]&pos,reg_1, (*cpu).reg[reg_1]&pos);
						(*cpu).mem[(*cpu).mem[(*cpu).counter+offset]] = (*cpu).reg[reg_1];
						break;
				case 12:
						reg_2 = (((*cpu).mem[(*cpu).counter])>>6)&7;
						(*cpu).counter++;
						if (reg_2!=7)
						{
							printf("JMP	R%x			; R7 <- x%x	; PC <- R%x = x%x\n",reg_2,(*cpu).counter, reg_2, (*cpu).reg[reg_2]);
							(*cpu).reg[7] = (*cpu).counter;
							(*cpu).counter = (*cpu).reg[reg_2];
						}
						else
						{
							printf("JMP	R%x			; PC <- R%x = x%x\n",reg_2, reg_2, (*cpu).reg[7]);
							(*cpu).counter = (*cpu).reg[7];
						}
						break;
				case 13:
						(*cpu).counter++;
						printf("ERR				; Reserved opcode; ignored\n");
						break;
				case 14:
						offset = (*cpu).mem[(*cpu).counter]&511;
						(*cpu).counter++;
						changeCC(cpu, ((*cpu).counter+offset)&pos);
						printf("LEA	R%x, %09d		; R%x <- x%x		; CC = %c\n",reg_1, offset, reg_1, (*cpu).counter+offset,(*cpu).CC);
						(*cpu).reg[reg_1] = (*cpu).counter+offset;
						break;
				case 15:
						offset = (*cpu).mem[(*cpu).counter]&255;
						(*cpu).counter++;
						if(offset==32)//x20
						{
							printf("TRAP x%x - GETC: ",offset);
							char answer;
							scanf("%c",&answer);
							(*cpu).reg[0] = answer;
							printf("R0 <- %c\n", (*cpu).reg[0]);
							//(*cpu).reg[0]; 
						}
						else if(offset==33)//x21
						{
							printf("TRAP x%x - OUT\n",offset);
							printf("%c\n", (*cpu).reg[0]);
						}
						else if(offset==34)//x22
						{
							printf("TRAP x%x - PUTS\n",offset);
							printf("%d\n",(*cpu).reg[0]);
						}
						else if(offset==35)//x23
						{
							printf("TRAP x%x - IN Enter a char (and/or press return): ",offset);
							char answer;
							scanf("%c",&answer);
							(*cpu).reg[0] = answer;
							printf("R0 <- %c\n", (*cpu).reg[0]);
						}
						else if(offset==37)//x25
						{
							printf("TRAP x%x - HALT\n",offset);
							printf("Halting execution.\n");
							(*cpu).running = 0;
						}
						else //x24 or others
						{
							printf("Wrong Trap Vector.\n");
							(*cpu).running = 0;
						}
						
						break;
				default:
						printf("error\n");
						break;
			}
			//increment PC
			//(*cpu).counter++;
			(*cpu).f++;
			
			if ((*cpu).counter > 65535)
			{
				(*cpu).counter = 0;
			}
		}
	}		  
}

// Execute the halt instruction (make CPU stop running)
//
void exec_HLT(CPU *cpu) {
    printf("HALT\nHalting\n");
    cpu -> running = 0;
}

// dump_CPU(CPU *cpu): Print out the control unit
// and general-purpose registers
// 
void dump_CPU(CPU *cpu) {
    // *** STUB ****
	int num = (*cpu).counter; 
	int po = (1<<16)-1;
	num = num&po;// pc 
	(*cpu).pc = (*cpu).mem[0]; 
	printf("PC: x%04x IR:  x%04x  RUNNING:	%d	CC: %c\n",num, (*cpu).mem[(*cpu).counter], (*cpu).running, (*cpu).CC);	
	dump_registers(cpu);
}

void dump_memory(CPU *cpu,int loc) 
{
    // *** STUB ****
	int f = 0;
	int address = 0;
	int num = (*cpu).mem[0];
	
	if (num < 0)//in case the starting position is negative
	{
		int po = (1<<16)-1;
		num = num&po;	
	}
	printf("MEMORY (from x%04x)\n", num);
	for (int i = 0; i<loc-1; i++)
	{
		int pos = (1<<16)-1;
		//int PC = 0;
		if (address==0) //getting starting position
		{
			(*cpu).pc = (*cpu).mem[f]&pos;
			//printf("%x",PC);
			address++;
			f++;
		}
		//if ((*cpu). mem [f]==0) //if the value stored is 0, we skip and go to next memory location
		//{
		//	f++;
		//}
		//printf("@ %02d	%04x\n", f, (*cpu).mem[f]);
		
		//skips if the instruction is 0
		if ((*cpu).mem[f]==0)
		{
			f++;
			num++;
		}
		else 
		{
			if ((*cpu).mem[f]<0)
			{	
				printf("x%04x:	x%04x	", num++, (*cpu).mem[f] & pos);
			}
			else
			{
				printf("x%04x:	x%04x	", num++, (*cpu).mem[f]);
			}
			int opcode;
			int reg_1;
			int reg_2;
			int reg_3;
			int flag;
			int offset;
			
			//int reg_num;
			//int z;
			//char str[20];
			//if ((*cpu).ir >= 0)//for positive instructions
			 // {
			if ((*cpu).mem[f]<0)
			{					
				opcode = (((*cpu).mem[f]&pos)>>12)&15;
				reg_1 = (((*cpu).mem[f]&pos)>>9)&7;
				//sprintf(str, "%x", (*cpu).mem[f]&pos);
				//sscanf(str, "%01x%03x", &opcode, &reg_1);
				//printf("\n x %x y %x \n",opcode,reg_1);
			} 
			else
			{
				opcode = (((*cpu).mem[f])>>12)&15;
				reg_1 = (((*cpu).mem[f])>>9)&7;
				//sprintf(str, "%x", (*cpu).mem[f]);
				//sscanf(str, "%01x%03x", &x, &reg_1);
				//printf("x%x, y%x \n",opcode,reg_1);
			}
			
			switch (opcode)
			{
				case 0:
						printf("%04d	BR \n",(*cpu).mem[f]);
						break;
				case 1:
						reg_2 = (((*cpu).mem[f])>>6)&7;
						flag = (((*cpu).mem[f])>>5)&1;
						reg_3 = (*cpu).mem[f]&7;
						offset = (*cpu).mem[f]&31;
						//printf("%d\n",flag);
						if (flag==0)
						{
							printf("%04d	ADD	R%x, R%x, R%x\n",(*cpu).mem[f], reg_1, reg_2, reg_3);
						}
						else
						{
							printf("%04d	ADD	R%x, R%x, %d \n",(*cpu).mem[f], reg_1, reg_2, offset);
						}
						break;
				case 2: 
						offset = (*cpu).mem[f]&511;
						printf("%04d	LD	R%x, %09d\n",(*cpu).mem[f], reg_1, offset);
						break;
				case 3:
						offset = (*cpu).mem[f]&511;
						printf("%04d	ST	R%x, %09d\n",(*cpu).mem[f], reg_1, offset);
						break;
				case 4:
						reg_2 = (((*cpu).mem[f])>>6)&7;
						flag = (((*cpu).mem[f])>>11)&1;
						offset = (*cpu).mem[f]&2047;
						if (flag == 1)
						{
							printf("%04d	JSR	%011d\n",(*cpu).mem[f],offset);
						}
						else
						{
							printf("%04d	JSRR	R%x\n",(*cpu).mem[f],reg_2);
						}
						break;
				case 5:
						reg_2 = (((*cpu).mem[f])>>6)&7;
						flag = (((*cpu).mem[f])>>5)&1;
						reg_3 = (*cpu).mem[f]&7;
						offset = (*cpu).mem[f]&31;
						if (flag==0)
						{
							printf("%04d	AND	R%x, R%x, R%x\n",(*cpu).mem[f], reg_1, reg_2, reg_3);
						}
						else
						{
							printf("%04d	AND	R%x, R%x, %d\n",(*cpu).mem[f], reg_1, reg_2, offset);
						}
						break;
				case 6:
						reg_2 = (((*cpu).mem[f])>>6)&7;
						offset = (*cpu).mem[f]&63;
						printf("%04d	LDR	R%x, R%x, %06d\n",(*cpu).mem[f], reg_1, reg_2, offset);
						break;
				case 7:
						reg_2 = (((*cpu).mem[f])>>6)&7;
						offset = (*cpu).mem[f]&63;
						printf("%04d	STR	R%x, R%x, %06d\n",(*cpu).mem[f], reg_1, reg_2, offset);
						break;
				case 8: 
						printf("%04d	RTI  \n",-(*cpu).mem[f]);
						break;
				case 9: 
						reg_2 = (((*cpu).mem[f])>>6)&7;
						printf("%04d	NOT	R%x, R%x\n",-(*cpu).mem[f],reg_1, reg_2);
						break;
				case 10:
						offset = (*cpu).mem[f]&511;
						printf("%04d	LDI	R%x, %09d\n",-(*cpu).mem[f],reg_1,offset);
						break;
				case 11:
						offset = (*cpu).mem[f]&511;
						printf("%04d	STI	R%x, %09d\n",-(*cpu).mem[f],reg_1,offset);
						break;
				case 12:
						reg_2 = (((*cpu).mem[f])>>6)&7;
						if (reg_2!=7)
						{
							printf("%04d	JMP	R%x\n",-(*cpu).mem[f],reg_2);
						}
						else
						{
							printf("%04d	JMP	R%x\n",-(*cpu).mem[f],reg_2);
						}
						break;
				case 13:
						printf("%04d	err  \n",-(*cpu).mem[f]);
						break;
				case 14:
						offset = (*cpu).mem[f]&511;
						printf("%04d	LEA	R%x, %09d\n",-(*cpu).mem[f], reg_1, offset);
						break;
				case 15:
						printf("%04d	TRAP  \n",-(*cpu).mem[f]);
						break;
				default:
						printf("error\n");
						break;
			}
			f++;
			//PC = PC+1;
			
			if (num > 65535)
			{
				num = 0;
			}
		}
		
	}	
}
		


void dump_registers(CPU *cpu) {
    // *** STUB ****
	int i = 0;
	while (i<NREG)
	{
		printf("R%d: %d	", i, (*cpu).reg[i]);
		i++;
	}
	printf("\n");
	
}
