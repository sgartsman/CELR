void read_memory(int fd, pid_t pid, off_t offset, int len, void *buffer)
{	
	ptrace(PTRACE_ATTACH, pid, 0, 0);
	waitpid(pid, NULL, 0);
	pread(fd, buffer, len, offset);
	ptrace(PTRACE_DETACH, pid, 0, 0);
}

void read_memory_no_ptrace(int fd, pid_t pid, off_t offset, int len, void *buffer)
{	
	pread(fd, buffer, len, offset);
}
