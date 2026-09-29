#define MEMSIZE 4096
#jumptomain NONE

// Right side of a comparison that is an expression: in brackets, or starting
// with a number.  Both used to fail ("')' expected" / "operator identifier
// expected"); a byte register on the left with 1 + b on the right hung the
// compiler.  Constant right sides are still folded (cmp [a],8).

#define SZ 16

dword a, b, c, r;
byte g, g2;

void main()
{
	if (a == (b + c)) r = 1;
	if (a < (b << 1) + c) r = 2;
	if ((a + 2) == (b)) && (c < (a)) r = 3;
	if (a == 1 + b) r = 4;
	if (a == 1 + (b * 2)) r = 5;
	if (a == SZ / 2) r = 6;
	if (g == (a + 4)) r = 7;
	if (g * c <= 9 + b) r = 8;
	AL = 9;
	if (AL > 1 + b) r = 9;
	for (r = 0; r < (a + b); r++) {}
	// byte expressions with brackets are evaluated in EAX
	g2 = (g + 4) & 0x0F;
	g2 = g + (a * 2);
	$ret
}
