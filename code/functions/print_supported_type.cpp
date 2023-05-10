void print_supported_types(supported_types i, char data_type)
{
		if (data_type=='c') {cout << (int) i.c;}
		if (data_type=='i') {cout << i.i;}
		if (data_type=='f') {cout << i.f;}
		if (data_type=='l') {cout << i.l;}
		if (data_type=='s') {cout << i.s;}
		if (data_type=='d') {cout << i.d;}
		cout << endl; return;
}
