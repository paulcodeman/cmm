#define MEMSIZE 65536

#jumptomain NONE

dword index;
dword values[2];

void main()
{
	dword local_value = 0;
	++local_value;
	--local_value;
	index = 0;
	++values[index];
	--values[index];
	for (index = 0; index < 2; ++index) {
		++values[index];
	}
}
