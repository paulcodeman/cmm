#define MEMSIZE 4096

#jumptomain NONE

dword a;
dword subtract_raw_result, subtract_reference_result;
dword add_raw_result, add_reference_result;

void main()
{
	a = 17;
	subtract_raw_result = subtract_raw(a);
	subtract_reference_result = subtract_reference(a);
	add_raw_result = add_raw(a);
	add_reference_result = add_reference(a);
	$ret
}

:dword subtract_raw(dword value)
{
	return 100 - value % 5 * 3;
}

:dword subtract_reference(dword value)
{
	return 100 - ((value % 5) * 3);
}

:dword add_raw(dword value)
{
	return 100 + value % 5 * 3;
}

:dword add_reference(dword value)
{
	return 100 + ((value % 5) * 3);
}
