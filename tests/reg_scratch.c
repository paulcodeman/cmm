#define MEMSIZE 4096
#jumptomain NONE

// A register target whose right operand binds tighter is computed through
// EAX (and EDX/ECX for * / % and shift counts).  Those registers are saved
// around it: KolibriOS code loads EAX (the function number) first.
// Check with -lst: push eax ... pop eax around each tighter operand, only
// for the registers that operand really uses.

dword a, b, c, w, x;

void main()
{
	EAX = 13;
	EDX = a << 16 | b << 8 | c;	// push eax / pop eax only
	EBX = (a << 16) + w;		// computed in EBX itself, no EAX: same code as the legacy a << 16 + w
	ECX = w + x * c;		// push eax, push edx
	EBX = a << (x + 1);		// shift count in CL: ECX saved too
	ECX = a << (x + 1);		// target ECX itself
	EDX = a + (b + c);
	$int 0x40
	$ret
}
