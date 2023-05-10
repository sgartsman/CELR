supported_types interpret_input(char data_type, std::string &input)
{
	supported_types rt;
	if (data_type=='c') {rt.c = (char) atoi(input.c_str());}
	if (data_type=='i') {rt.i = atoi(input.c_str());}
	if (data_type=='f') {rt.f = atof(input.c_str());}
	if (data_type=='l') {rt.l = atol(input.c_str());}
	if (data_type=='s') {rt.s = (short) atoi(input.c_str());}
	if (data_type=='d') {rt.d = (double) atof(input.c_str());}
	return rt;
}
