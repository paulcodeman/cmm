#define MEMSIZE 4096
#jumptomain NONE

dword a,b;

void main()
{
	EAX = 0;
	EBX = ((a<<16)+b);
	$int 0x40
	$ret
}
