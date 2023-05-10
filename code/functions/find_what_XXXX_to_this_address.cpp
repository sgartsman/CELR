// DR7 - DEBUG REGISTER 7
// read more - https://en.wikipedia.org/wiki/X86_debug_register
// the following enums and struct are supposed to give easy access to interpret and change DR7 corretly
enum DR7_debug_mode
{
    DR7_BREAK_ON_EXEC  = 0,
    DR7_BREAK_ON_WRITE = 1,
    DR7_BREAK_ON_RW    = 3,
};

enum DR7_len
{
    DR7_LEN_1 = 0,
    DR7_LEN_2 = 1,
    DR7_LEN_4 = 3,
    DR7_LEN_8 = 2, // only on processors x64
};

struct debug_register6
{
    char b0;
};
typedef debug_register6 dr6_t;

struct debug_register7
{
    char l0:1;
    char g0:1;
    char l1:1;
    char g1:1;
    char l2:1;
    char g2:1;
    char l3:1;
    char g3:1;
    char le:1;
    char ge:1;
    char pad1:3;
    char gd:1;
    char pad2:2;
    char rw0:2;
    char len0:2;
    char rw1:2;
    char len1:2;
    char rw2:2;
    char len2:2;
    char rw3:2;
    char len3:2;
};
typedef debug_register7 dr7_t;

void find_what_writes_to_this_address(off_t address, char data_type, int reads, pid_t pid)
{
	// would not print anyways, and causes bugs in the process
	if (reads < 1) 
	{
		return;
	}
	
	// attach to target
	ptrace(PTRACE_ATTACH, pid, 0, 0);
	waitpid(pid, 0, 0);
	
	// setup new dr7
	dr7_t dr7 = {0};
	dr7.rw0 = DR7_BREAK_ON_WRITE; // only monitor writes
	dr7.l0 = 1; // only watch for this process, not any process
	dr7.len0 = DR7_LEN_4; // data len to look for
	
	// cout << "attached" << endl;
	
	// read: https://sites.uclouvain.be/SystInfo/usr/include/sys/user.h.html
	if (ptrace(PTRACE_POKEUSER, pid, offsetof(struct user, u_debugreg[7]), dr7)==-1)
	{
		exit(EXIT_FAILURE);
        }   
        
        // cout << "updated dr7" << endl;
	
	// setup new dr0
	long dr0 = (long) address;
	if (ptrace(PTRACE_POKEUSER, pid, offsetof(struct user, u_debugreg[0]), dr0) ==-1)
	{
		perror("ptrace");
	}
            
	// cout << "updated dr0" << endl;
	
	// gain access to process memory
	char file[64];
	sprintf(file, "/proc/%ld/mem", (long)pid);
	int fd = open(file, O_RDWR);
	
	ptrace(PTRACE_CONT, pid, 0, 0);
	
	FILE *fp;
	unsigned long rip;
	char buffer[31]; // len 30: at least 2 full opcodes as longest opcode in 15
	while(reads>0)
	{
	
		// wait until process triggers debug trap and gets its RIP at this point
		waitpid(pid, 0, 0);
		rip = ptrace(PTRACE_PEEKUSER, pid, offsetof(struct user, regs.rip), &rip);
	
		// check wether or not the signal was duo to hitting our debug condition
		
		
		// print dissassembly and opcode of last executed instruction before the break		
		read_memory_no_ptrace(fd, pid, rip-30, 60, buffer);
		
		
		fp = fopen("./runtime/temp_code","wb");
		fwrite(buffer, 1, 30, fp);
		fclose(fp);
		
		cout << rip << ": " << exec("python3 ./runtime/trim_machine_code.py");

		if (reads > 1)
		{	
			ptrace(PTRACE_CONT, pid, 0, 0);
		}
		
		reads--;
	}
	
	// remove the break condition set earlier in dr7
	dr7.l0 = 0; 
	ptrace(PTRACE_POKEUSER, pid, offsetof(struct user, u_debugreg[7]), dr7);
	
	ptrace(PTRACE_DETACH, pid, 0, 0);
}

