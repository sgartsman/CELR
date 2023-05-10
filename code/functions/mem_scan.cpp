/* function mem_scan
in: pid of process to scan
in: list of addresses found on former scans
in: data type to scan for (ie: 'i' for int, 'f' for float, 'd' for double) strings not supported.
in: type of match (ie: '=' for exact matches with exact value, '>' for bigger than compared value. special: # for bigger than last scan. ^ for smaller than last scan.
in: value to match with
out: updates the address table to only addresses that fir the requirement */ 
void mem_scan(pid_t pid, addresses_list &addr_table, char data_type, char comparison_type, supported_types compare_to)
{

	// get value size by type
	int value_size = assess_type_size(data_type);
	if (value_size == 0)
	{
		return;
	}		

	// gain access to process memory
	char file[64];
	sprintf(file, "/proc/%ld/mem", (long)pid);
	int fd = open(file, O_RDWR);
	
	// if no previous addresses provided scan all memory ("First Scan")
	if (addr_table.empty())
	{
		// read memory mapping
		std::list<memory_map> *memory_maps = fetch_memory_map(pid);

		// iterate ove memory maps
		int i;
		std::list<memory_map>::iterator it;
		scanned_addr sr;
		void *scanned_addr, *ss;
		for (it = memory_maps->begin(); it != memory_maps->end(); ++it)
		{	
			// read memory of each map
	    		void *buffer = malloc((int)it->end_addr - (int)it->start_addr + 1);
	    		read_memory(fd, pid, it->start_addr, ((int) it->end_addr) - ((int) it->start_addr), buffer);
	    				

			// iterate over every object of the type in the mapping
			for (i=0; it->start_addr + (i*value_size) <= it-> end_addr - value_size; i++)  // end_addr - value_size is max address of last object
			{				
				scanned_addr = (void *)((char *)buffer+(i*value_size)); // local address of the copy of the value of the address scanned (see ss)
				ss = (void *) (it->start_addr + (i*value_size)); // address scanned on the target process
				memcpy(&sr.val_when_scanned.c, scanned_addr, value_size); 

				if (test_data(sr.val_when_scanned, data_type, comparison_type, compare_to))
				{
					// if address was found as potential match add it to the list
					sr.addr = (off_t) ss;				
					addr_table.push_front(sr);
					
					cout << "addr: " << sr.addr << ", val: ";
					print_supported_types(sr.val_when_scanned, data_type);
				}
			}
			free (buffer);	    		
	    	}
	    	
		delete memory_maps;
		close(fd);
		return;
	}
	
	// memory addresses provided - "scan again"
	// re scan on addresses found on last scan
	int i;
	scanned_addr sr;
	int len = addr_table.size();
	for (i=0; i<len; i++)
	{	
		sr = addr_table.front(); // address and value of scanned address
		addr_table.pop_front(); // remove address from potential addresses

		read_memory(fd, pid, sr.addr, value_size, &sr.val_when_scanned.c); // update the value in this address
		
		if (test_data(sr.val_when_scanned, data_type, comparison_type, compare_to))
		{
			// if value is still relevent return it to the back of the list
			addr_table.push_back(sr);
			cout << "addr: " << sr.addr << ", val: ";
			print_supported_types(sr.val_when_scanned, data_type);
		}
	}
	
	return;
}

