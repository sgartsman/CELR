void write_memory(int fd, pid_t pid, off_t offset, int len, void *buffer)
{
	ptrace(PTRACE_ATTACH, pid, 0, 0); // attach as debugger
	waitpid(pid, NULL, 0); // wait for process to stop
	pwrite(fd, buffer, len, offset); // write data
	ptrace(PTRACE_DETACH, pid, 0, 0); // deatach
}


