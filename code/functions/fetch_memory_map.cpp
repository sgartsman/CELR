/*
func fetch_memory_maps
in: pid of process
out: list of memory mapped regions in it
*/
std::list<memory_map> *fetch_memory_map(pid_t pid)
{
	// set up maps list
	std::list<memory_map> *memory_maps = new std::list<memory_map>;

	// get maps from bash
	std::string command = "cat /proc/" + std::to_string((int) pid) + "/maps";
	const char *cmd = command.c_str();	
	char mem_buffer[256];
	char num_buffer[256];
	std::string result = "";
	FILE* pipe = popen(cmd, "r");

	// iterate each map and append to list
	int i, j;
	memory_map map;
	
	while (fgets(mem_buffer, sizeof(mem_buffer), pipe) != NULL)
	{
		// fetch starting addr (start to '-')
		for (i=0; mem_buffer[i] != '-'; i++)
		{
			num_buffer[i] = mem_buffer[i];
		}
		map.start_addr = (off_t) strtol(num_buffer, NULL, 16);
		
		// fetch ending addr (after '-' til ' ')
		i++; // skip the '-'
		for (j=0; mem_buffer[i] != ' '; i++)
		{
			num_buffer[j] = mem_buffer[i];
			j++;
		}
		num_buffer[j+1] = '\0';
		map.end_addr = (off_t) strtol(num_buffer, NULL, 16);
		
		memory_maps->push_back(map);
	}
	pclose(pipe);
	
	// dismiss last map as it is irrelevent kernal space
	memory_maps->pop_back();
    	
    	return memory_maps;
}
