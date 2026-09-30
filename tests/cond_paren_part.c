#define MEMSIZE 4096
#jumptomain NONE

// A condition part that is a bare operand in brackets, followed by && or ||.
// The ')' after the operand used to be taken as the start of arithmetic and
// the whole condition turned into "always true" with broken code after it
// (Eolite include/gui.h: for (i=0; (PathShow_path[i]) && (i<...); i++)).

dword r, x;

void main()
{
	dword i;
	char buf[16];
	r = 0;
	for (i=0; (buf[i]) && (i<15); i++) r++;
	for (i=0; (buf[i]) || (i>15); i++) r++;
	for (i=0; (buf[i]); i++) r++;
	for (i=0; (x); i++) r++;
	i = 0;
	while ((buf[i]) && (i<15) && (x)) i++;
	if ((buf[i]) && (x)) r = 3;
	if ((buf[i])) r = 4;
	$ret
}
