dword a,b;

dword foo(dword x)
{
	return x * 2;
}

void main()
{
	b = 3;
	a = foo(b) + 1;
}
