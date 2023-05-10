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

find_what_writes_to_this_address(address, data_type, reads, pid)
{
	std::string input;
	
	// get address
	cout << "address: \n >> ";
	cin >> input;
	long addr = (long) atol(input.c_str());
	
	// get number of reads
	cout << "# of reads: \n >> ";
	cin >> input;
	int reads = atoi(input.c_str());
	
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
	long dr0 = (long) addr;
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
	
	unsigned long rip;
	unsigned char buffer[61] = {0}; // len 60: max opcode is len 15 and for accurate results need to include at least two opcodes before and one after target command (3 before rip + rip)
	while(reads>0)
	{
		// wait until process triggers debug trap and gets its RIP at this point
		waitpid(pid, 0, 0);
		rip = ptrace(PTRACE_PEEKUSER, pid, offsetof(struct user, regs.rip), &rip);

		// print dissassembly and opcode of last executed instruction before the break		
		read_memory_no_ptrace(fd, pid, rip-60, 60, buffer);
		
		FILE *fp;
		fp = fopen("../runtime/temp_code","wb");
		fwrite(buffer, 1, 80, fp);
		fclose(fp);
		
		cout << rip << ": " << exec("python3 ../runtime/trim_machine_code.py");
		
		if (reads > 1) {ptrace(PTRACE_CONT, pid, 0, 0);}
		
		reads--;
	}
	
	// dont watch for this breakpoint anymore
	dr7.l0 = 0; 
	ptrace(PTRACE_POKEUSER, pid, offsetof(struct user, u_debugreg[7]), dr7);
	
	// print dissassembly and opcode of last executed instruction before the break		
	read_memory_no_ptrace(fd, pid, rip-60, 60, buffer);
	
	FILE *fp;
	fp = fopen("../runtime/temp_code","wb");
	fwrite(buffer, 1, 80, fp);
	fclose(fp);
	
	cout << rip << ": " << exec("python3 ../runtime/trim_machine_code.py");
	
	ptrace(PTRACE_DETACH, pid, 0, 0);
}

