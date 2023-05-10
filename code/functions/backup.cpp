#define err_pp if(status==-1){cout<<"error"<<endl;}

off_t find_what_writes_to_this_address(off_t address, char data_type, pid_t pid)
{
	// gain access to process memory
	char file[64];
	sprintf(file, "/proc/%ld/mem", (long)pid);
	int fd = open(file, O_RDWR);	
	
	// assess data size
	int data_size = assess_type_size(data_type);
	
	// save address of first command to be executed in the loop
	struct user_regs_struct regs;
	off_t last_executed_command;
	off_t next_executed_command;
	ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
	next_executed_command = (off_t) regs.rip;
	
	// get a copy of what the data originally was
	supported_types original_data;
	read_memory(fd, pid, address, data_size, &original_data.c);
	print_supported_types(original_data, 'i');
	
	int status;
		
	// freeze the target process and attach to it
	status=ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	err_pp
	status=waitpid(pid, NULL, 0);
	err_pp

	supported_types current_data;
	while (true)
	{
		cout << "a\n";
		// allow target process to make a single step (execute 1 opcode)
		status = ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
		if(!status) {waitpid(pid, &status, 0);}
		
		// update commands addresses
		last_executed_command = next_executed_command;
		status = ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
		err_pp
		cout << "b\n";
		next_executed_command = (off_t) regs.rip;
		
		// check if data in target address was changed
		read_memory(fd, pid, address, data_size, &current_data.c);
		print_supported_types(current_data, 'i');
		if(!test_data(original_data, data_type, '=', current_data))
		{
			// if changed, return address of the last executed command
			ptrace(PTRACE_DETACH, pid, nullptr, nullptr);
			close(fd);
			return last_executed_command;
		}
	}
	ptrace(PTRACE_DETACH, pid, nullptr, nullptr);
	close(fd);
	return 0;
}




void read_memory_no_ptrace(int fd, pid_t pid, off_t offset, int len, void *buffer);

off_t find_what_writes_to_this_address(off_t address, char data_type, pid_t pid)
{
	// gain access to process memory
	char file[64];
	sprintf(file, "/proc/%ld/mem", (long)pid);
	int fd = open(file, O_RDWR);	
	
	// assess data size
	int data_size = assess_type_size(data_type);
	
	// freeze the target process and attach to it
	ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	waitpid(pid, NULL, 0);
	
	// save address of first command to be executed in the loop
	struct user_regs_struct regs;
	off_t last_executed_command;
	off_t next_executed_command;
	ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
	next_executed_command = (off_t) regs.rip;
	
	// get a copy of what the data originally was
	supported_types original_data;
	read_memory_no_ptrace(fd, pid, address, data_size, &original_data.c);
	print_supported_types(original_data, 'i');

	supported_types current_data;
	while (true)
	{
		//cout << "a\n";
		// allow target process to make a single step (execute 1 opcode)
		ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
		waitpid(pid, NULL, 0);
		
		// update commands addresses
		last_executed_command = next_executed_command;
		ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
		// cout << "b\n";
		next_executed_command = (off_t) regs.rip;
		
		// check if data in target address was changed
		read_memory_no_ptrace(fd, pid, address, data_size, &current_data.c);
		//print_supported_types(current_data, 'i');
		if(!test_data(original_data, data_type, '=', current_data))
		{
			// if changed, return address of the last executed command
			ptrace(PTRACE_DETACH, pid, nullptr, nullptr);
			close(fd);
			return last_executed_command;
		}
	}
	ptrace(PTRACE_DETACH, pid, nullptr, nullptr);
	close(fd);
	return 0;
}

void read_memory_no_ptrace(int fd, pid_t pid, off_t offset, int len, void *buffer)
{
	pread(fd, buffer, len, offset);
}
