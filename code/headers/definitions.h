class memory_map
{
	public:
	off_t start_addr;
	off_t end_addr;
};

template <typename T> class scanned_memory_address
{
	public:
	off_t addr;
	T val_when_scanned; // mem_cpy value when scan into this buffer
};

union supported_types
{
	char c;
	unsigned char C;
	int i;
	unsigned int I;
	float f;
	long l;
	unsigned long L;
	short s;
	unsigned short S;
	double d;
};
typedef scanned_memory_address<supported_types> scanned_addr;
typedef std::list<scanned_addr> addresses_list;

// le big functions
void mem_scan(pid_t pid, addresses_list &addr_table, char data_type, char comparison_type, supported_types compare_to);
void find_what_writes_to_this_address(off_t address, char data_type, int reads, pid_t pid);
std::list<memory_map> *fetch_memory_map(pid_t pid);

// general utility
std::string exec(const char* cmd);
int assess_type_size(char type);
void print_addresses_list(addresses_list &addr_table,char data_type);

//supported_tpyes util functions
bool test_data(supported_types data, char data_type, char comparison_type, supported_types compare_to);
supported_types interpret_input(char data_type, std::string &input);
void print_supported_types(supported_types i, char data_type);

// read & write memory
void read_memory(int fd, pid_t pid, off_t offset, int len, void *buffer);
void write_memory(int fd, pid_t pid, off_t offset, int len, void *buffer);
void read_memory_no_ptrace(int fd, pid_t pid, off_t offset, int len, void *buffer);

