/*
func assess_type_size
in: char that represents a type
out: size of the type */
int assess_type_size(char type)
{
	if (type=='c') {return sizeof(char); }
	if (type=='i') {return sizeof(int); }
	if (type=='f') {return sizeof(float); }
	if (type=='l') {return sizeof(long); }
	if (type=='s') {return sizeof(short); }
	if (type=='d') {return sizeof(double); }
	return 0;
}
