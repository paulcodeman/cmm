#define MEMSIZE 4096
#jumptomain NONE

dword g, ga, gb, gc, gd;

:stdcall dword sink(dword a, b, c)
{
	return a + b + c;
}

:dword test_args()
{
	dword a, b, c, d;
	a = ga;
	b = gb;
	c = gc;
	d = gd;
	return sink(a, b + c * d, d);
}

:dword test_parens()
{
	dword a, b, c, d;
	a = ga;
	b = gb;
	c = gc;
	d = gd;
	if (((a - b) / c) + d == 8) return 1;
	return 0;
}

void main()
{
	ga = 10;
	gb = 3;
	gc = 2;
	gd = 5;
	g = test_args();
	g += test_parens();
	$ret
}
