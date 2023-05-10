void print_addresses_list(addresses_list &addr_table,char data_type)
{
	for (auto const &j: addr_table) 
	{
		supported_types i = j.val_when_scanned;
		cout << "addr: " << j.addr << ", val: ";
		if (data_type=='c') {cout << (int) i.c;}
		if (data_type=='i') {cout << i.i;}
		if (data_type=='f') {cout << i.f;}
		if (data_type=='l') {cout << i.l;}
		if (data_type=='s') {cout << i.s;}
		if (data_type=='d') {cout << i.d;}
		cout << endl;	
	}
}
