#define MEMSIZE 4096

#jumptomain NONE

dword value;

void main()
{
	value = 1;
	// This comment must end at an LF newline too.
	value += 2;
	$ret
}
