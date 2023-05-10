#include "./headers/includes.h"

int debug = 0;

using std::cout, std::cin, std::endl;

#include "./headers/definitions.h"

/* function comm
in: command entered in STD::in
in: pid of attached process
in: refferance to adresses found in former scans
out: executes entered command */
void comm(std::string command, pid_t pid, addresses_list &addr_table)
{	
	if (command == "scan")
	{
		std::string input;
		cout << "-== input data type ==-" << endl; // << "--> ";
		cin >> input;
		char data_type = input.c_str()[0];
		cout << "-== input comparison type ==-" << endl; // << "--> ";
		cin >> input;
		char comparison_type = input.c_str()[0];
		cout << "-== input comparison to ==-" << endl; // << "--> ";
		cin >> input;
		supported_types val = interpret_input(data_type, input);

		mem_scan(pid, addr_table, data_type, comparison_type, val);
		return;
	}
	
	if (command == "write")
	{
		std::string input;
		cout << "-== input address ==-" << endl; // "--> ";
		cin >> input;
		off_t address = (off_t) atol(input.c_str());
		cout << "-== input data type ==-" << endl; // "--> ";
		cin >> input;
		char data_type = input.c_str()[0];
		cout << "-== input new data ==-" << endl; // "--> ";
		cin >> input;
		supported_types new_data = interpret_input(data_type, input);
		
		// gain access to process memory
		char file[64];
		sprintf(file, "/proc/%ld/mem", (long)pid);
		int fd = open(file, O_RDWR);
		
		write_memory(fd, pid, address, assess_type_size(data_type), &new_data.c);
		
		close(fd);
		return;
	}
	
	if (command == "read")
	{
		std::string input;
		cout << "-== input address ==-" << endl; // "--> "
		cin >> input;
		off_t address = (off_t) atol(input.c_str());
		cout << "-== input data type ==-" << endl; // "--> "
		cin >> input;
		char data_type = input.c_str()[0];
		
		// gain access to process memory
		char file[64];
		sprintf(file, "/proc/%ld/mem", (long)pid);
		int fd = open(file, O_RDWR);
		
		supported_types data_read;
		read_memory(fd, pid, address, assess_type_size(data_type), &data_read.c);
		print_supported_types(data_read, data_type);
		
		close(fd);
		return;
	}
	
	if (command == "fww" or command == "FWW" or command == "find_what_writes") // find what writes 
	{
		std::string input;
		cout << "-== input address ==-" << endl; // "--> ";
		cin >> input;
		off_t address = (off_t) atol(input.c_str());
		cout << "-== input data type ==-" << endl; // "--> ";
		cin >> input;
		char data_type = input.c_str()[0];
		cout << "-== input number of reads ==-" << endl; //"--> ";
		cin >> input;
		int reads = atoi(input.c_str());
		
		find_what_writes_to_this_address(address, data_type, reads, pid);
		return;
	}
	
	if (command == "reset")
	{
		while (!addr_table.empty())
		{	
			addr_table.pop_front();
		}
		return;
	}
	
	if (command == "print")
	{
		std::string input;
		cout << "-== input data type ==-" << endl; //<< "--> ";

		cin >> input;
		char data_type = input.c_str()[0];
		print_addresses_list(addr_table, data_type);
		return;
	}
	
	
	cout << "unknown command!" << endl;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cout << "use: tracer [pid]" << endl;
		return 1;
	}
	
	// get target process PID from argv[1]
	pid_t pid;
	pid = (pid_t)atoi(argv[1]);

	// create the list of addresses for scans
	addresses_list addr_table;
	
	// main program loop: ask for command, execute, repeat.
	std::string input;
	while(true)
	{
		cout << "-== input command ==-" << endl; // << "--> ";
		cin >> input;
		comm(input, pid, addr_table);
	}
		
	return 0;
}

// all untility functions used
#include "./functions/find_what_XXXX_to_this_address.cpp"
#include "./functions/print_supported_type.cpp"
#include "./functions/exec.cpp"
#include "./functions/mem_scan.cpp"
#include "./functions/fetch_memory_map.cpp"
#include "./functions/assess_type_size.cpp"
#include "./functions/test_data.cpp"
#include "./functions/read_memory.cpp"
#include "./functions/write_memory.cpp"
#include "./functions/print_addresses_list.cpp"
#include "./functions/interpret_input.cpp"

