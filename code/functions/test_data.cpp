bool compare_ints(int data, char comparison_type, int compare_to);
bool compare_doubles(double data, char comparison_type, double compare_to);

/*
in: pointer to var of a certein type
in: type of data
in: type of test (i.e =, <, >)
in: data (same type) to compare to
out: true if data passed the test
*/
bool test_data(supported_types data, char data_type, char comparison_type, supported_types compare_to)
{
	switch(data_type)
	{
		case 'i': return compare_ints(data.i, comparison_type, compare_to.i);
		case 'd': return compare_doubles(data.d, comparison_type, compare_to.d);
	}	

	return false;
}


bool compare_ints(int data, char comparison_type, int compare_to)
{
	switch(comparison_type)
	{
		case '=': return data==compare_to;
		case '>': return data>compare_to;
		case '<': return data<compare_to;
	}
	return false;
}

bool compare_doubles(double data, char comparison_type, double compare_to)
{
	switch(comparison_type)
	{
		case '=': return  -0.1 < data-compare_to && data-compare_to < 0.1;
		case '>': return data>compare_to;
		case '<': return data<compare_to;
	}
	return false;
}
