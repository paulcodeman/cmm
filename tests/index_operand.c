#define MEMSIZE 4096
#jumptomain NONE

// An indexed operand that starts a tighter term: its index (bufrm) used to be
// dropped before the term was compiled, so a + arr[x] * c read arr[0].

dword a, c, x, p, r;
dword arr[4];

void main()
{
	r = a + arr[x] * c;
	r = a | arr[x] << 1;
	r = a - arr[x] / c;
	r = a + DSDWORD[p + 8] * c;
	EBX = a + arr[x] * c;
	$ret
}
