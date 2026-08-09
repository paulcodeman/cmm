#define MEMSIZE 65536

#jumptomain NONE

byte byte_value;
word word_value;
dword dword_value;
qword qword_value;
float float_value;
double double_value;

void main()
{
	++byte_value;
	--byte_value;
	++word_value;
	--word_value;
	++dword_value;
	--dword_value;
	++qword_value;
	--qword_value;
	++float_value;
	--float_value;
	++double_value;
	--double_value;
	++AL;
	--AL;
	++AX;
	--AX;
	++EAX;
	--EAX;
	++EDX:EAX;
	--EDX:EAX;
	qword_value++;
	qword_value--;
	EDX:EAX++;
	EDX:EAX--;
	$ret
}
